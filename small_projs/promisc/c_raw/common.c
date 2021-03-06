#include <string.h>

#include "common.h"

inline void
print_usage(char *pname)
{
	printf("USAGE:\n%s [-l] INTERFACE\n", pname);
}

inline void *
malloc_or_die(size_t sz)
{	
	void *tmp;
	if ((tmp = malloc(sz)) == NULL)
		print_warn_and_die("malloc_err");
	return tmp;
}

inline void *
ustrdup(char *str)
{
	char *tmp;

	tmp = strdup(str);
	if (tmp == NULL)
		print_warn_and_die("no mem");
	return tmp;
}

