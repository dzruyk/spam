#ifndef DFT_HEADER

#define DFT_HEADER

int dft(float *input, int input_sz, float *rex, float *imx);

int dft_rev(float *rex, float *imx, int input_sz, float *output);

int rectangle_to_polar(float *rex, float *imx, int input_sz, float *mag, float *phase);

int polar_to_rectangle(float *mag, float *phase, int input_sz, float *rex, float *imx);

#endif

