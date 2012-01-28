
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "convert.h"
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
	uint16_t *sbuf;
	float *rex, *fft_ibuf, *fft_obuf;
	int res, sbuf_sz, fsz;
	int i;
	
	sbuf_sz = winsz * sizeof(short);
	fsz = winsz * sizeof(float);

	sbuf = malloc(sbuf_sz);
	if (sbuf == NULL)
		goto err;
	
	rex = malloc(fsz);
	if (rex == NULL)
		goto err;

	fft_ibuf = malloc(2 * fsz);
	if (fft_ibuf == NULL)
		goto err;
	
	fft_obuf = malloc(2 * fsz);
	if (fft_obuf == NULL)
		goto err;
	
	
	while ((res = fread(sbuf, 1, sbuf_sz, fp)) == sbuf_sz) {
		memset(fft_ibuf, 0, 2 * fsz);
		
		int16_to_float(sbuf, rex, winsz);
		float_scale_down(rex, winsz);
		
		/*for (i = 0; i < 2 * winsz; i += 2)
			fft_ibuf[i] = rex[i >> 1];
		*/
		smbPitchShift(1.0, winsz, 256, 4, 44100, rex, fft_obuf);
		
		
		/*for (i = 0; i < 2 * winsz; i += 2)
			rex[i >> 1] = fft_obuf[i];
		*/
		for (i = 0; i < winsz; i++)
			rex[i] = fft_obuf[i];
		
		float_scale_up(rex, winsz);
		float_to_int16(rex, sbuf, winsz);
		
		write(fileno(stdout), sbuf, winsz * 2);
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
