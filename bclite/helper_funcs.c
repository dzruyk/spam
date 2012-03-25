#include <stdio.h>
#include <stdlib.h>

inline void
print_usage(char *pname)
{
	printf("USAGE:%s\n", pname);
}

inline void
print_warn_and_die(char *str)
{
	fprintf(stderr, "%s", str);
	exit(1);
}

inline void
print_warn(char *str)
{
	fprintf(stderr, "%s", str);
}

inline void *
malloc_or_die(size_t sz)
{	
	void *tmp;
	if ((tmp = malloc(sz)) == NULL)
		print_warn_and_die("malloc_err");
	return tmp;
}

