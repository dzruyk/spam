#include <stdio.h>
#include <string.h>

#include "common.h"

enum {
	RC_MULT = 101,
};


/*
 * Knuth - Morris - Pratt algo
 * 
 * description on
 * http://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
 */
void
get_pref(int *pref, char *str)
{
	int i, j;

	pref[0] = 0;

	j = 0;

	for (i = 1; i < strlen(str); i++) {
		while (j > 0 && str[i] != str[j]) 
			j = pref[j - 1];
		if (str[i] == str[j])
			j++;
		pref[i] = j;
	}
}

//BUGGGS!

int
kmp_search(char *big, char *small)
{
	int i;
	int j;
	int blen, slen;
	int *pref;

	pref = malloc_or_die(strlen(small));

	get_pref(pref, small);

	blen = strlen(big);
	slen = strlen(small);

	if (slen > blen)
		return -1;
	
	j = 0;

	for (i = 0; i < blen; i++) {
		if (big[i] != small[j]) {
			j = pref[j];
			continue;
		} else
			j++;
		//end!
		if (j == slen)
			return i - slen + 1;
	}

	free(pref);
	
	return -1;
}

/*
 * Robin Carp Algo realisation
 */
static int
rc_hash(char *s, int len)
{
	int i;
	int res;

	res = 0;

	for (i = 0; i < len; i++)
		res += s[i] * RC_MULT;

	return res;	
}

#define rc_hash_char(chr) chr * RC_MULT

int
rabin_carp_search(char *big, char *small)
{
	int i;
	int shash, bhash;
	int blen, slen;

	blen = strlen(big);
	slen = strlen(small);

	if (slen > blen)
		return -1;

	shash = rc_hash(small, slen);
	bhash = rc_hash(big, slen);

	for (i = 0; i < blen - slen + 1; i++) {
		if (shash == bhash &&
		    strncmp(big + i, small, slen) == 0)
			return i;
		bhash = bhash - rc_hash_char(big[i]) 
		    + rc_hash_char(big[i + slen]);
	}

	return -1;
}

int
trivia_search(char *big, char *small)
{
	int i, j;
	int blen, slen;

	blen = strlen(big);
	slen = strlen(small);

	for (i = 0; i < blen - slen + 1; i++) {
		for(j = 0; j < slen; j++) {
			if (big[i + j] != small[j])
				break;
		}
		if (j == slen)
			return i;
	}

	return -1;
}

