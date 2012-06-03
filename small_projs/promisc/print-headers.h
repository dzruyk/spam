#ifndef __PRINT_HEADERS_H__
#define __PRINT_HEADERS_H__

/*
 * Main print function
 */
void print_main(void *data, int len);

/*
 * prints ethernet header, that
 * must be contained in the first argu
 *
 * WARNING: now i not print extended header
 *
 */
void print_eth_header(void *data);

/*
 * Just prints data contained in the first argu
 * lenght of data must be at second argu
 */
void print_raw_hex_data(void *data, int len);

#endif

