
#ifndef _LNLY_CONVERT
#define _LNLY_CONVERT

/*
 * convert input (int16_t) arr into float
 * nelem is number of elements in arrs
 *
 */

/* README: Надо запилить какуюнибудь универсальную функцию. */

void int16_to_float(uint16_t *input, float *output, uint32_t nelem);

void float_to_int16(float *input, uint16_t *output, uint32_t nelem);


void float_scale_down(float *input, uint32_t nelem);

void float_scale_up(float *input, uint32_t nelem);

#endif

