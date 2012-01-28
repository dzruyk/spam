/*
 * Extremely simple FFT algorithm.
 *
 * Copyleft (C) 2011  Grisha Sitkarev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fft.h"

static unsigned int
lb(int n)
{
	unsigned int x, c;

	x = 0;
	c = 1;

	while (c < n) {
		c <<= 1;
		++x;
	}

	return x;
}

int
fft(float *x, int n, int invert)
{
	unsigned int i, j, k;
	unsigned int le, le2, n2, nlev;
	float sr, si, wr, wi, tr, ti;

	if (x == NULL || n < 0)
		return -1;

	/* Do reverse bit sorting in the input. */
	for (i = 0; i < n; i++) {
		unsigned int cnt, a;
		float *p, *p2, tmp;

		a = 0;
		for (cnt = 1; cnt < n; cnt <<= 1) {
			a <<= 1;
			if (i & cnt)
				a |= 0x1;
		}

		if (a > i) {
			p = x + (a << 1);
			p2 = x + (i << 1);
			tmp = *p;
			*p++ = *p2;
			*p2++ = tmp;
			tmp = *p;
			*p = *p2;
			*p2 = tmp;
		}
	}


	nlev = lb(n);
	n2 = n / 2;
	le = 1;

	for (i = 0; i < nlev; i++) {
		float *p, *p2;
		float arg;

		le2 = le;
		le <<= 1;

		arg = M_PI / le2;
		wr = cos(arg);
		wi = (invert ? 1.0 : -1.0) * sin(arg);

		p = x;
		p2 = p + le;

		for (j = 0; j < n; j += le) {
			float *pr, *pi, *p2r, *p2i;

			/* Initialize twiddle factor. */	
			sr = 1.0;
			si = 0.0;

			for (k = 0; k < le2; k++) {
				pr = p++;
				pi = p++;
				p2r = p2++;
				p2i = p2++;
				/* Butterfly summation for even and odd parts. */
				tr = *p2r*sr - *p2i*si;
				ti = *p2r*si + *p2i*sr;
				*p2i = *pi - ti;
				*p2r = *pr - tr;
				*pr += tr;
				*pi += ti;
				/* Calculate next twiddle factor. */
				tr = sr*wr - si*wi;
				si = sr*wi + si*wr;
				sr = tr; 
			}

			p += le;
			p2 += le;
		}
	}

	return 0;
}

