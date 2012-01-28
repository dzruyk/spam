#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "pitch_shift.h"
#include "fft.h"


int enable_pitch(float *fft_ibuf, float *fft_obuf, int n);
void print_warn_and_die(char *str);
void save_arr(float *arr, int n, char *name);

#define SHIFT_FAC 2.0
enum {
	IBUF_SZ = 512,
	STEP = 128,
};

int bufsz;
int step;

int
main(int argc, char *argv[])
{
	float *obuf, *ibuf;
	int i, fsz;

	// мега кривой кусок но лень напрягацо
	if (argv[1] != NULL) {
		bufsz = atoi(argv[1]);
		if (argv[2] != 0)
			step = atoi(argv[2]);
		else
			step = STEP;
	}
	else {
		bufsz = IBUF_SZ;
		step = STEP;
	}



	fsz = bufsz * sizeof(float);
	
	obuf = malloc(fsz);
	if (obuf == NULL)
		goto err;
	
	ibuf = malloc(fsz);
	if (ibuf == NULL)
		goto err;
	
	/* Now we must fill buf with some data^_^ */
	memset(ibuf, 0, bufsz * sizeof(*ibuf));

	for (i = 0; i < bufsz; i++) {
	//	ibuf[i] += sin(2 * M_PI * 4 * i / bufsz);
	//	ibuf[i] += sin(2 * M_PI * 0.5 * i / bufsz);
	//	ibuf[i] += sin(2 * M_PI * 1.5 * i / bufsz);
		ibuf[i] += 1;
	//	ibuf[i] += sin(2 * M_PI * 8 * i / (bufsz + 500));
	//	ibuf[i] += sin(2 * M_PI * (bufsz + 2) * i / (bufsz * 4));
	//	ibuf[i] += sin(2 * M_PI * 1023.0 * i / bufsz);
	}


	enable_pitch(ibuf, obuf, bufsz);
	
	save_arr(ibuf, bufsz, "input.dat");
	save_arr(obuf, bufsz, "output.dat");

	//
	{
		float work_td[8192];
		float mag[8192], phase[8192];
		int tmp;
		
		for (i = 0; i < bufsz; i++) {
			work_td[i * 2] = ibuf[i];
			work_td[i * 2 + 1] = 0;
		}
		fft(work_td, bufsz, 0);
		
		for (i = 0; i < bufsz / 2; i++) {
			int rex, imx;

			rex = work_td[i * 2];
			imx = work_td[i * 2 + 1];

			mag[i] = 2.0 * sqrt(rex * rex + imx * imx);
			phase[i] = atan2(imx, rex);
		}
		//
		FILE *ma, *ph;
		ma = fopen("input_mag%d.dat", "w");
		ph = fopen("input_phase.dat", "w");
		
		for (i = 0; i < bufsz / 2; i++) {
			fprintf(ma, "%.8f\n", mag[i]);
			fprintf(ph, "%.8f\n", phase[i]);
		}
		fclose(ma);
		fclose(ph);
	}
	//
	
	free(ibuf);
	free(obuf);

	return 0;

	err:
	print_warn_and_die("malloc_err\n");
	return -1;
}

int
enable_pitch(float *fft_ibuf, float *fft_obuf, int n)
{
	int i;
		
	for (i = 0; i < n; i += n)
		smbPitchShift(1, n, step, 4, 1., fft_ibuf + i, fft_obuf + i);

	return 0;

}

void
save_arr(float *arr, int n, char *name)
{
	FILE *fp;
	int i;

	fp = fopen(name, "w");
	if (fp == NULL)
		print_warn_and_die("fopen err\n");
	
	for (i = 0; i < n; i++)
		fprintf(fp, "%.8f\n", arr[i]);
	
	fclose(fp);
}


void
print_warn_and_die(char *str)
{
	fprintf(stderr, "%s", str);
	exit(1);
}

