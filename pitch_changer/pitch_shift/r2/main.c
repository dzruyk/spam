
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "pitch_shift.h"


int enable_pitch(FILE *fp, int winsz);
int is_pow2(int num);
void print_usage(char *pname);
void print_warn_and_die(char *str);


int
main(int argc, char *argv[])
{
	FILE *fp;
	int sz;

	if (argc < 2) {
		print_usage(argv[0]);
		return 1;
	}
	
	fp = fopen(argv[1], "r");
	if (fp == NULL)
		print_warn_and_die("fopen err(\n");
	
	if (argc >= 3)
		sz = atoi(argv[2]);
	else
		sz = 1024;
	
	if (is_pow2(sz) == 0)
		print_warn_and_die("size is not pow of 2\n");

	enable_pitch(fp, sz);

	return 0;
}

int
enable_pitch(FILE *fp, int winsz)
{
	float *fft_ibuf, *fft_obuf;
	int res, fsz;
	int i;
	
	fsz = winsz * sizeof(float);

	fft_ibuf = malloc(fsz);
	if (fft_ibuf == NULL)
		goto err;
	
	fft_obuf = malloc(fsz);
	if (fft_obuf == NULL)
		goto err;
	
	
	while ((res = fread(fft_ibuf, 1, fsz, fp)) == fsz) {
		
		smbPitchShift(1.3, winsz, winsz, 1, 44100, fft_ibuf, fft_obuf);

		write(fileno(stdout), fft_obuf, fsz);
	}

	return 0;

	err:
	print_warn_and_die("malloc_err\n");
	return 0;
}


int
is_pow2(int num)
{
	if (num < 2)
		return 0;
	
	if ((num & (num -1)) != 0)
		return 0;
	return 1;
}

void
print_usage(char *pname)
{
	printf("USAGE:\n %s INPUT_FILE [winsz]\n", pname);
	printf("winsz is number [2^1...2^31]\n");
}

void
print_warn_and_die(char *str)
{
	fprintf(stderr, "%s", str);
	exit(1);
}
