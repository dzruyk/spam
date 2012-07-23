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
static void
get_pref(int *pref, const char *str)
{
	int i, j;

	pref[0] = 0;

	memset(pref, 0, strlen(str) * sizeof(int));

	j = 0;

	for (i = 2; i < strlen(str); i++) {
		while (j > 0 && str[i - 1] != str[j]) 
			j = pref[j];
		if (str[i - 1] == str[j])
			j++;
		pref[i] = j;
	}
}

//BUGGGS!
int
kmp_search(const char *big, const char *small)
{
	int i;
	int j;
	int blen, slen;
	int *pref;

	pref = malloc_or_die(strlen(small) * sizeof(int));

	get_pref(pref, small);

	blen = strlen(big);
	slen = strlen(small);

	if (slen > blen)
		return -1;
	
	j = 0;

	for (i = 0; i < blen; i++) {
		while (j > 0 && big[i] != small[j]) {
			j = pref[j];
		} 
		if (big[i] == small[j])
			j++;
		//end!
		if (j == slen)	{
			free(pref);
			return i - slen + 1;
		}
	}

	free(pref);
	
	return -1;
}

/*
 * Robin Carp Algo realisation
 */
static int
rc_hash(const char *s, int len)
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
rabin_carp_search(const char *big, const char *small)
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
trivia_search(const char *big, const char *small)
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


int
strstr_search(const char *s, const char *subs)
{
	char *res;
	res = strstr(s, subs);

	if (res == NULL)
		return -1;
	else
		return res - s;
}	


