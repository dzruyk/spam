#include <stdio.h>

#include "helper.h"


stat_t
get_new_line(char *s, int sz, FILE *fp)
{
	char *res;

	res = fgets(s, sz, fp);
	if (res == NULL) {
		if (feof(fp) != 0)
			return stat_eof;
		else
			return stat_err;
	}

	return stat_ok;
}


