#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef int boolean_t;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define print_warn_and_die(fmt, arg...) \
do { \
	fprintf(stderr, "%s %d %s warning:"fmt, \
			__FILE__, \
			__LINE__, \
			__FUNCTION__, \
			##arg); \
	exit(1); \
} while (0)



#define print_warn(fmt, arg...) \
do { \
	fprintf(stderr, "%s %d %s warning:"fmt, \
			__FILE__, \
			__LINE__, \
			__FUNCTION__, \
			##arg); \
	fflush(stderr); \
} while (0)



typedef enum {  
	ret_not_found     = -6,	
	ret_entry_exists  = -5,
        ret_no_access     = -4,
        ret_out_of_memory = -3,
        ret_invalid       = -2,
        ret_err           = -1,
        ret_ok            = 0,
        ret_file_exists   = 1,
        ret_not_file      = 2,
} ret_t;

inline void print_usage(char *pname);

inline void *malloc_or_die(size_t sz);

inline void *ustrdup(char *str);

#endif /*COMMON_H_*/

