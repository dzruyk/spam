#include "pitch_shift.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum {
	TRUE = 1,
	FALSE = 0,
	MAX_BUF_SIZE = 8192,
};

/* Delete me when you end */
static void save_arr(float *arr, int n, char *name);


void 
pitch_shift(float shift, int sz, int N, float smpl_rate, float *input, float *output)
{
	float work_td[2 * MAX_BUF_SIZE];
	float proc_mag[MAX_BUF_SIZE / 2];
	float proc_freq[MAX_BUF_SIZE / 2];
	float syn_mag[MAX_BUF_SIZE / 2];
	float syn_freq[MAX_BUF_SIZE / 2];
	static float sum_phase[MAX_BUF_SIZE / 2];
	static float last_phase[MAX_BUF_SIZE / 2];
	static int init = TRUE;
	
	double rex, imx, mag, phase;
	double expct, tmp, fpb;					
	
	int i, k, ind, N2, ppp;				// ppp is PI per phase
	
	//
	expct = 2. * M_PI;
	fpb = smpl_rate / N;
	N2 = N / 2;
	
	if (init == TRUE) {
		memset(last_phase, 0, MAX_BUF_SIZE / 2 * sizeof(float));	
		memset(sum_phase, 0, MAX_BUF_SIZE / 2 * sizeof(float));
		init = FALSE;
	}
	
	/* for every sample in window */
	for (k = 0; k < sz; k += N) {
		printf("k = %d\n", k);
		for (i = 0; i < N; i++) {
			work_td[i * 2] = input[i + k];
			work_td[i * 2 + 1] = 0;
		}

		/* Dirrect fft */
		fft(work_td, N, 0);

		/* Processing step */
		for (i = 0; i < N2; i++) {

			rex = work_td[i * 2];
			imx = work_td[i * 2 + 1];

			mag = 2.0 * sqrt(rex * rex + imx * imx);
			phase = atan2(imx, rex);
			
			/* Get true freq. */
			
			//phase difference
			tmp = phase - last_phase[i];
			last_phase[i] = phase;
			
			// substract expected phase
			tmp -= i * expct;
			
			//map phase difference into -PI;PI interval
			ppp = tmp / M_PI;
			if (ppp >= 0)
				ppp += ppp & 1;
			else
				ppp -= ppp & 1;
			tmp -= M_PI * ppp;
			
			//
			tmp = tmp / (2 * M_PI);
			tmp = i * fpb + tmp * fpb;

			proc_mag[i] = mag;
			proc_freq[i] = tmp;
		}
		
		/* Pitch_shift */
		memset(syn_mag, 0, MAX_BUF_SIZE / 2);
		memset(syn_freq, 0, MAX_BUF_SIZE / 2);

		for (i  = 0; i < N2; i++) {
			ind = i * shift;
			if (ind < N2) {
				syn_mag[ind] += proc_mag[i];
				syn_freq[ind] = proc_freq[i] * shift;
			}
		}
		
		/* Syntesis step */
		for (i = 0; i < N2; i++) {
			mag = syn_mag[i];
			tmp = syn_freq[i];
			
			tmp -= i * fpb;
			
			tmp /= fpb;
			
			//
			tmp = tmp * 2 * M_PI;
			tmp += i * expct;
			
			sum_phase[i] += tmp;
			phase = sum_phase[i];
			
			work_td[i * 2] = mag * cos(phase);
			work_td[i * 2 + 1] = mag * sin(phase);
		}
		
		/* zero neg freqs */
		for (i = N2 + 2; i < 2 * N; i++)
			work_td[i] = 0.0;
		
		/* Normalize step. */
		for (i = 0; i < N; i++) {
			work_td[i * 2] /= N;
			work_td[i * 2 + 1] /= N;
		}
		work_td[0] /= 2;
		work_td[2 * N - 2] /= 2;

		/* rev fft */
		fft(work_td, N, 1);

		
		for (i = 0; i < N; i++)
			output[i + k] = work_td[i << 1];

	}
}

/*
		//
		{	
			char name[20];
			char name2[20];
			sprintf(name, "syn_mag%d.dat", k);
			sprintf(name2, "syn_freq%d.dat", k);
			printf("%s %d\n", name, k);
			save_arr(syn_mag, N2, name);
			save_arr(syn_freq, N2, name2);
		}
		//
		
 */


/* Delete me when you end */
static void
save_arr(float *arr, int n, char *name)
{
	FILE *fp;
	int i;

	fp = fopen(name, "w");
	if (fp == NULL)
		return;
	
	for (i = 0; i < n; i++)
		fprintf(fp, "%.8f\n", arr[i]);
	
	fclose(fp);
}


