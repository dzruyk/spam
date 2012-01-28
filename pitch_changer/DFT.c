#include <math.h>
#include <stdlib.h>
#include <string.h>


static void ampl_sync(float *rex, float *imx, int input_sz);

/*
 * в функцию вставлена ampl_sync (раньше она была в dft_reverse)
 */
int
dft(float *input, int input_sz, float *rex, float *imx)
{
	int i, j, outsz;
 
	outsz = input_sz / 2;

	for (i = 0; i < outsz; i++)
		rex[i] = imx[i] = 0.0;
	
	for (i = 0; i < outsz; i++)
		for(j = 0; j < input_sz; j++) {
			rex[i] += input[j] * cos(2 * M_PI * j * i/ input_sz);
			imx[i] += -1 * input[j] * sin(2 * M_PI * j * i/ input_sz);
		}
	
	ampl_sync(rex, imx, input_sz/2);
	
	return 0;
}


int
dft_rev(float *rex, float *imx, int input_sz, float *output)
{
	int i, j, outsz;
	 
	outsz = input_sz * 2;

	for (i = 0; i < outsz; i++)
		output[i] = 0.0;
	
	for (i = 0; i < outsz; i++)
		for(j = 0; j < input_sz; j++) {
			output[i] += rex[j] * cos(2 * M_PI * j * i/ outsz);
			output[i] += imx[j] * sin(2 * M_PI * j * i/ outsz);
		}
	
	return 0;
}


static void
ampl_sync(float *rex, float *imx, int input_sz)	/* пишу перед экзаменом. мб баг - не правильно считаются амл. */
{							/* надо домножить на 2 */ 
	int i;
	
	for (i = 0; i < input_sz; i++) {
		rex[i] = rex[i] / (input_sz);
		imx[i] = -1 * imx[i] / (input_sz);
	}
	
	rex[0] /= 2;
	rex[input_sz -1] /= 2;
}

					/* АХТУНГ! работает с freq domain вместо амплитуд(х.з. насколько правильно) */
int
rectangle_to_polar(float *rex, float *imx, int input_sz, float *mag, float *phase)
{
	int i;
	
	for (i = 0; i < input_sz; i++) {
		mag[i] = sqrt(rex[i] * rex[i] + imx[i] * imx[i]);
		if (rex[i] == 0) {
			if (imx[i] > 0) {
				phase[i] = M_PI / 2;
				continue;
			} else {
				phase[i] = - M_PI / 2;
				continue;
			}
		}
		phase[i] = atan2(imx[i], rex[i]);
	}
	return 0;
}

int
polar_to_rectangle(float *mag, float *phase, int input_sz, float *rex, float *imx)
{
	int i;
	
	for (i = 0; i < input_sz; i++) {
		rex[i] = mag[i] * cos(phase[i]);
		imx[i] = mag[i] * sin(phase[i]);
	}
	return 0;
}
