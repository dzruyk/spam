#ifndef _HELPER_H_
#define _HELPER_H_
#include <stdio.h>
#include <stddef.h>

inline void print_usage(char *pname);

inline void print_warn_and_die(char *str);

inline void print_warn(char *str);

inline void *malloc_or_die(size_t sz);

#endif

