#include <stdint.h>

void
int16_to_float(uint16_t *input, float *output, uint32_t nelem)
{
	int i;

	for (i = 0; i < nelem; i++)
		*(output++) = *(input++);

}

void
float_to_int16(float *input, uint16_t *output, uint32_t nelem)
{
	int i;

	for (i = 0; i < nelem; i++)
		*(output++) = *(input++);

}

void
float_scale_down(float *input, uint32_t nelem)
{
	int i;

	for (i = 0; i < nelem; i++)
		*(input++) /= 32768;
}

void
float_scale_up(float *input, uint32_t nelem)
{
	int i;

	for (i = 0; i < nelem; i++)
		*(input++) *= 32768;
}