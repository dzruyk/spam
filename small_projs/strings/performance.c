#include <sys/time.h>

#include <stdio.h>
#include <string.h>

#include "search.h"
#include "common.h"

enum {
	S_LEN = 4096,
	SUBS_LEN = 1024,
	CYCELS = 1024,
};


void
diff_time(struct timeval *result, struct timeval *a, struct timeval *b)
{
	int oflg, usec, sec;

	oflg = 0;

	usec = a->tv_usec - b->tv_usec;
	
	if (a->tv_usec < b->tv_usec) {
		usec += 1000000;
		oflg = 1;
	}

	sec = a->tv_sec - b->tv_sec - oflg;

	if (a->tv_sec < b->tv_sec) {
		sec += 60 * 60 * 24;
	}

	
	result->tv_usec = usec;
	result->tv_sec = sec;
}

#define ARR_SZ(val) sizeof(val)/sizeof(*val)

struct func_name {
	int (*func)(const char *s, const char *subs);
	char *fname;
} functions[] = {
	{trivia_search, "trivia_search"},
	{strstr_search, "strstr_search"},
	{rabin_carp_search, "rabin_carp_search"},
	{kmp_search, "kmp_search"},
};

int
main(int argc, char *argv[])
{
	struct timeval start, stop, result;
	char *s;
	char *subs;
	int i, j, ret;


	s = malloc_or_die(S_LEN);
	subs = malloc_or_die(SUBS_LEN);

	memset(s, 'a', S_LEN);
	memset(subs, 'a', SUBS_LEN);

	subs[SUBS_LEN - 2] = 'b';
	s[S_LEN - 2] = 'b';

	s[S_LEN - 1] = subs[SUBS_LEN - 1] = 0;

	for (i = 0; i < ARR_SZ(functions); i++) {

		ret = gettimeofday(&start, NULL);
		if (ret != 0)
			print_warn_and_die("gettime err");

		for (j = 0; j < CYCELS; j++) {
			functions[i].func(s, subs);
		}

		ret = gettimeofday(&stop, NULL);
		if (ret != 0)
			print_warn_and_die("gettime err");
		
		diff_time(&result, &stop, &start);

		printf("%s: seconds = %d.%d\n", functions[i].fname, result.tv_sec, result.tv_usec);
	}

	free(s);
	free(subs);

	return 0;
}

