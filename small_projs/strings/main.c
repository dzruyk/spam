#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search.h"

#define TEST_FILE "res.txt"
#define STR_MAX 512

char *s = "this is main string with substring";
char *subs = "substring";

char *s2 = "this is main string with substring";
char *subs2 = "this";

void
print_warn_and_die(char *str)
{
	printf("%s", str);
	exit(1);
}

int
strstr_search(char *s, char *subs)
{
	char *res;
	res = strstr(s, subs);

	if (res == NULL)
		return -1;
	else
		return res - s;
}	

int
main(int argc, char *argv[])
{

	printf("strstr=%d\nrabin_carp=%d\ntrivia=%d\n",
	    strstr_search(s2, subs2),
	    rabin_carp_search(s2, subs2),
	    trivia_search(s2, subs2));
	printf("strstr=%d\nrabin_carp=%d\ntrivia=%d\n",
	    strstr_search(s, subs),
	    rabin_carp_search(s, subs),
	    trivia_search(s, subs));
	
	//now automatic checks need to run
	//input file look like this
	//STRING1
	//SUBSTRING1
	//STRING2
	//SUBSTRING2
	
	while (1) {
		char s[STR_MAX];
		char subs[STR_MAX];
		char *res;

		FILE *fp;

		fp = fopen(TEST_FILE, "r");
		if (fp == NULL)
			print_warn_and_die("open error\n");
		
		//CHECK ERRORS, BITCH
		res = fgets(s, STR_MAX, fp);
		if (res == NULL) {
			if (ferror(fp) != 0)
				print_warn_and_die("fgets err\n");
			else
				print_warn_and_die("unexpected_eof\n");
		}
				
		res = fgets(subs, STR_MAX, fp);
		if (res == NULL)
			if (feof(fp) != 0)
				break;
			else
				print_warn_and_die("fgets err\n");
	}

	return 0;
}

