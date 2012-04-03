#ifndef _HELPER_H_
#define _HELPER_H_
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

inline void print_usage(char *pname);


inline void print_warn_and_die(char *str);

#define print_warn_and_die(fmt, arg...) \
do { \
	fprintf(stderr, "warning:"fmt, \
			##arg); \
	exit(1); \
} while (0)



#define print_warn(fmt, arg...) \
do { \
	fprintf(stderr, "warning:"fmt, \
			##arg); \
	fflush(stderr); \
} while (0)

inline void *malloc_or_die(size_t sz);

#endif

