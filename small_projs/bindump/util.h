#ifndef _UTIL_H_
#define _UTIL_H_

#define CRLF	"\r\n"

#define error(status, fmt, arg...) \
do { \
	fprintf(stderr, "error: %s:%d %s: "fmt CRLF, \
			__FILE__, \
			__LINE__, \
			__FUNCTION__, \
			##arg); \
	fflush(stderr); \
	exit(status);	\
} while (0)

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

#define ARRSZ(arr) (sizeof(arr) / sizeof(arr[0]))

int xfseek(FILE *stream, long offset, int whence);
void *xmalloc(size_t sz);
void *xrealloc(void *ptr, size_t sz);


long xftell(FILE *stream);
int fread_all(FILE *f, char *buf, uint32_t sz);
int get_file_size(FILE *f);

#endif

