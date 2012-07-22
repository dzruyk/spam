#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search.h"
#include "common.h"

#define TEST_FILE "./res.txt"
#define STR_MAX 512


char *predef[][2] = {
		     {"+-+-+-+-+\n", "-+-+-+\n"},
		     {"this is main string with substring", "substring"},
		     {"this is main string with substring", "this"},
		     {"in the middle right now!", "middle"},
		     {"this is not match string", "why i cant find"},
		     {"substring", "long than string"},
		     {"substring", "substring2"},
		    };

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

int
main(int argc, char *argv[])
{
	int i;
	int is_err, sz;
	FILE *fp;

	is_err = 0;
	sz = sizeof(predef) / sizeof(*predef);
	//manual defined string tests
	//CHECK CORNER CASES
	for (i = 0; i < sz; i++)
		printf("%d) strstr=%d rabin_carp=%d trivia=%d"
		    " kmp_search=%d\n",
		    i,
		    strstr_search(predef[i][0], predef[i][1]),
		    rabin_carp_search(predef[i][0], predef[i][1]),
		    trivia_search(predef[i][0], predef[i][1]),
		    kmp_search(predef[i][0], predef[i][1])
		    );
	
	//now automatic checks need to run
	//input file look like this
	//STRING1
	//SUBSTRING1
	//STRING2
	//SUBSTRING2
	
	fp = fopen(TEST_FILE, "r");
	if (fp == NULL)
		print_warn_and_die("open error\n");

	i = 0;
	while (1) {
		int rc_res, t_res, str_res, kmp_res;
		char s[STR_MAX];
		char subs[STR_MAX];
		char *res;

		res = fgets(s, STR_MAX, fp);
		if (res == NULL) {
			if (feof(fp) != 0) {
				printf("EOF\n");
				break;
			}
			else
				print_warn_and_die("fgets err\n");
		}
				
		res = fgets(subs, STR_MAX, fp);
		if (res == NULL) {
			if (ferror(fp) != 0)
				print_warn_and_die("fgets err\n");
			else {
				printf("string = %d\n", i + 1);
				print_warn_and_die("unexpected_eof\n");
			}
		}

		rc_res = rabin_carp_search(s, subs);
		t_res = trivia_search(s, subs);
		kmp_res = kmp_search(s, subs);
		str_res = strstr_search(s, subs);

		if (rc_res != str_res) {
			printf("err!\nrc=%d str=%d\n s='%s' subs='%s'\n",
			    rc_res, str_res, s, subs);
			is_err = 1;
		}
			    
		if (t_res != str_res) {
			printf("err!\nt=%d str=%d\n s='%s' subs='%s'\n",
			    t_res, str_res, s, subs);
			is_err = 1;
		}

		if (kmp_res != str_res) {
			printf("err!\nkmp=%d str=%d\n s='%s' subs='%s'\n",
			    kmp_res, str_res, s, subs);
			is_err = 1;
		}

		
		i++;
	}
	
	if (is_err == 0)
		printf("last line num = %d\nSUCCESS!!\n", i * 2);

	return 0;
}

