#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "DFT.h"
#include "fft.h"

void write_signal(double *x, int n, char *dest);

enum {
	SAMPLES = 2048,
};


int
main(int argc, char *argv[])
{
	double XR[SAMPLES], XI[SAMPLES];
	double mag[SAMPLES], phase[SAMPLES];
	int i;
	
	for (i = 0; i < SAMPLES; i++)
		mag[i] = phase[i] = XI[i] = XR[i] = 0;
	
	double window;
	/* Sets XR. */
	for (i = 0; i < SAMPLES; i++) {
		XR[i] += 125 * sin(2 * M_PI * 5 * i/SAMPLES);

		window = -0.5 * cos(2.0 * M_PI * (double)i / (double)SAMPLES) + 0.5;
		XR[i] *= window;
	}
	
	write_signal(XR, SAMPLES, "./input_fft.dat");	
	
	fft(XR, XI, SAMPLES);

	write_signal(XR, SAMPLES, "./rex_fft.dat");	
	write_signal(XI, SAMPLES, "./imx_fft.dat");
	
	rectangle_to_polar(XR, XI, SAMPLES, mag, phase);
	write_signal(mag, SAMPLES, "./mag_fft.dat");
	write_signal(phase, SAMPLES, "./phase_fft.dat");
	
	fft_rev(XR, XI, SAMPLES);
	
	write_signal(XR, SAMPLES, "./output_re_fft.dat");	
	write_signal(XI, SAMPLES, "./output_im_fft.dat");
	
	
	return 0;
}


void
write_signal(double *x, int n, char *dest)
{
	FILE *fd;
	int i;
	fd = fopen(dest, "w");
	if (fd == NULL) {
		perror("fprint signal:");
		exit(1);
	}
	
	for (i = 0; i < n; i++)
		fprintf(fd, "%F\n", x[i]);
	
	fclose(fd);
}
