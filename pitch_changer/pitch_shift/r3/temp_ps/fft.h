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
#ifndef FFT_H_
#define FFT_H_

/*
 * Performs Fast Fourier Transform in-place.
 *
 * This algorithm uses radix-2 Cooley-Tukey FFT algorithm.
 *
 * Input and output signals are stored in array with real and imaginary
 * values in two consecutive array cells. See examples for details.
 *
 * x	   - input signal which is rewritten by output
 * n	   - number of samples in input signal
 * invert  - 0 for direct (DFT) and 1 for inverse (IDFT)
 *
 * Returns: 0 on success, -1 on failure.
 */

int fft(float *x, int n, int invert);

#endif

