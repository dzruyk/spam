#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef NULL
#define NULL (void *)0
#endif

#define ARRSZ(a) (sizeof(a)/sizeof(a[0]))

char *getprogname();

#endif
