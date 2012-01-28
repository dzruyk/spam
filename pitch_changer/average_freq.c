
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "convert.h"
#include "DFT.h"
#include "fft.h"

int average_freq(FILE *fp, int winsz);
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
		sz = 2048;
	
	if (is_pow2(sz) == 0)
		print_warn_and_die("size is not pow of 2\n");

	average_freq(fp, sz);

	return 0;
}

int
average_freq(FILE *fp, int winsz)
{
	uint16_t *sbuf;
	float *avmag, *imx, *rex, *phase, *mag;				//bufs of int16_t and float
	int res, rounds, sbuf_sz, fsz;
	int i;
	
	sbuf_sz = winsz * sizeof(short);
	fsz = winsz * sizeof(float);

	sbuf = malloc(sbuf_sz);
	if (sbuf == NULL)
		goto err;
	
	rex = malloc(fsz);
	if (rex == NULL)
		goto err;
	avmag = malloc(fsz);
	if (avmag == NULL)
		goto err;
	
	imx = malloc(fsz);
	if (imx == NULL)
		goto err;

	phase = malloc(fsz);
	if (phase == NULL)
		goto err;

	mag = malloc(fsz);
	if (mag == NULL)
		goto err;

	memset(avmag, 0, fsz);
	
	rounds = 0;

	while ((res = fread(sbuf, 1, sbuf_sz, fp)) == sbuf_sz) {
		int16_to_float(sbuf, rex, winsz);
		memset(imx, 0, fsz);

		fft(rex, imx, winsz);
		
		rectangle_to_polar(rex, imx, winsz, mag, phase);

		// sum all magnitudes
		for (i = 0; i < winsz; i++)
			avmag[i] += mag[i];

		rounds++;
	}
	
	if (rounds > 0)
		for (i = 0; i < winsz; i++)
			avmag[i] /= rounds;
	
	
	for (i = 1; i < winsz / 2; i++)
		printf("%F\n", avmag[i]);


	return 0;

	err:
	print_warn_and_die("malloc_err\n");
	
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

