#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "util.h"

inline int
xfseek(FILE *stream, long offset, int whence)
{
	int ret;

	ret = fseek(stream, offset, whence);
	if (ret == -1)
		error(1, "seek error");

	return ret;
}

inline long
xftell(FILE *stream)
{
	int ret;

	ret = ftell(stream);
	if (ret == -1)
		error(1, "ftell error");

	return ret;
}

inline void *
xmalloc(size_t sz)
{	
	void *tmp;
	if ((tmp = malloc(sz)) == NULL)
		error(1, "malloc_err");
	return tmp;
}

inline void *
xrealloc(void *ptr, size_t sz)
{
	void *tmp;
	if ((tmp = realloc(ptr, sz)) == NULL)
		error(1, "malloc_err");
	return tmp;
}

int
fread_all(FILE *f, char *buf, uint32_t sz)
{
	int ret;

	do {
		ret = fread(buf, 1, sz, f);
		if (ret <= 0 && !feof(f))
			return 1;
		sz -= ret;
		buf += ret;
	} while (sz > 0);
	
	return 0;
}

int
get_file_size(FILE *f)
{
	int curoff;
	int len;

	curoff = ftell(f);

	xfseek(f, 0, SEEK_END);

	len = ftell(f);

	xfseek(f, curoff, SEEK_SET);
	
	return len;
}

