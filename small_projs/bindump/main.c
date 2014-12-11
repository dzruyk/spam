#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <limits.h>

#include <unistd.h>

#include "extractor.h"
#include "log.h"
#include "util.h"

#define DUMP_NAME_FMT "./_dumped_%d.bin"
#define FSIZE_MAX 1024 * 1024 * 64
#define no_argument 0

char *pname;

struct buf {
	void *ptr;
	uint32_t sz;
	uint32_t allocated;
} filebuf = {0};

struct {
	int dumpflag; 

} prog_opts = {0};

void
usage()
{
	printf("Usage: %s [-upwh]\n"
	    "-u --url dump all urls to stdin\n"
	    "-p --pe search pe files\n"
	    "-w --write to disk finded binaries\n"
	    "-h --help print this help\n",
	    pname);
	exit(0);
}

void
read_file_into_buf(const char *fname, struct buf *buf)
{
	FILE *f;
	uint32_t sz;

	f = fopen(fname, "rb");
	if (f == NULL)
		error(1, "can't open %s", fname);

	sz = get_file_size(f);
	if (sz > FSIZE_MAX)
		sz = FSIZE_MAX;

	if (buf->allocated < sz) {
		buf->ptr = xrealloc(buf->ptr, sz);
		buf->allocated = sz;
	}

	buf->sz = sz;

	DEBUG(LOG_DEFAULT, "file size %d\n", buf->sz);

	if (fread_all(f, buf->ptr, buf->sz) != 0)
		error(1, "can't read file %s", fname);

	fclose(f);
}

typedef int (*searcher_t)(void *ptr, uint32_t maxsz);
typedef int (*dumper_t)(void *ptr, uint32_t maxsz);

int
dump_http(void *ptr, uint32_t sz)
{
	printf("Link %.*s\n", sz, (char *) ptr);

	return 0;
}

//TODO: need to dump correctly if file is under encoding
int
dump_binary(void *ptr, uint32_t maxsz)
{
	FILE *f;
	char dstfname[PATH_MAX];

	static int dumped = 0;

	if (prog_opts.dumpflag == 0)
		return 0;

	DEBUG(LOG_DEFAULT, "dump binary\n");

	snprintf(dstfname, sizeof(dstfname), DUMP_NAME_FMT, dumped);

	f = fopen(dstfname, "wb");
	if (f == NULL)
		error(1, "fopen error");

	//FIXME: stop to be so optimistic
	fwrite(ptr, 1, maxsz, f);

	printf("%s dumped to disk", dstfname);

	dumped++;

	return 0;
}

void
scan_file(const char *fname)
{
	int i, j;
	void *ptr;
	uint32_t sz;

	struct {
		searcher_t fsearcher;
		dumper_t fdumper;
	} handlers[] = {
		{.fsearcher = get_http,		.fdumper = dump_http},
		{.fsearcher = get_exe,		.fdumper = dump_binary},
		{.fsearcher = get_elf,		.fdumper = dump_binary},
	};

	read_file_into_buf(fname, &filebuf);

	ptr = filebuf.ptr;
	sz = filebuf.sz;

	//FIXME: search only subfiles(self dump bypass)
	for (i = 1, ptr++; i < sz; i++, ptr++) {
		for (j = 0; j < ARRSZ(handlers); j++) {
			int n;

			n = handlers[j].fsearcher(ptr, filebuf.sz - i);
			if (n != 0)
				handlers[j].fdumper(ptr, n);
		}
	}
}

int
main(int argc, char *argv[])
{
	int c;
	struct option long_opts[] = {
		{"url",		no_argument, 0, 'u'},
		{"pe",		no_argument, 0, 'p'},
		{"write",	no_argument, 0, 'w'},
		{"help",	no_argument, 0, 'h'},
		{NULL,		no_argument, 0,  0 },
	};

	pname = argv[0];
	
	while (TRUE) {
		c = getopt_long(argc, argv, "upwh", long_opts, NULL);
		if (c == -1)
			break;

		//FIXME: just stub now
		switch (c) {
		case 'u':
			break;
		case 'p':
			break;
		case 'w':
			prog_opts.dumpflag++;
			break;
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	while (argc--) {
		scan_file(*argv++);
	}

	return 0;
}

