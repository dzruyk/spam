#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "macros.h"
#include "blowfish.h"


// static uint32_t newP[] = {
// 	0xFFFFFFFF, 0x85A308D3, 0x13198A2E, 0x03707344,
//         0xA4093822, 0x299F31D0, 0x082EFA98, 0xEC4E6C89,
//         0x452821E6, 0x38D01377, 0xBE5466CF, 0x34E90C6C,
//         0xC0AC29B7, 0xC97C50DD, 0x3F84D5B5, 0xB5470917,
//         0x9216D5D9, 0x8979FB1B
// };
// static uint32_t key[] = {
// 	0x12345678, 0xFFFFFFFF, 0xAAAABBBB
// };
// 
// static uint32_t var[]= { 0xdeadbeef, 0xabcdef};
// 
// static unsigned char var2[] = {0xde, 0xad, 0xbe, 0xef, 0xab, 0xcd, 0xef, 0x00};




int
get_mode(char *arg)
{
	int i;
	int res;
	int tmp;
	res = 0;
	while (*arg != '\0') {
		tmp = *(arg++) - '0';
		if (tmp < 0 || tmp > 9) {
			printf("NOT_NUMBER\n");
			exit(4);
		}
		res = res * 10 + tmp;
	}
	
	return res;
}

uint32_t buff[2];

int
main(int argc, char *argv[])
{
	int i, mode;
	FILE *of, *inf;
	void (*f)(struct blowfish_context *ctx, uint32_t *block);
	int n;
	struct blowfish_context *ctx;
	
	if (argc < 4) {
		printf("usage:\n program input_file output_file mode(0-for encr 1-for decr)\n");
		exit(1);
	}
	
	inf = fopen(argv[1], "rb");
	if (inf == NULL) {
		perror("FOPEN_INPUT:");
		exit(2);
	}
	
	of = fopen(argv[2], "w");
	if (of == NULL) {
		perror("FOPEN_INPUT:");
		exit(2);
	}
	
	mode = get_mode(argv[3]);
	
	if (mode != 0 && mode != 1) {
		printf("MODE_ERROR\n");
		exit(1);
	}
	
	ctx = blowfish_context_new();

	blowfish_set_key(ctx, (unsigned char*) "TESTKEY", 7);
	
	
	if (mode == 0)
		f = blowfish_encrypt;
	else 
		f = blowfish_decrypt;
	
	i = 0;
	printf("\n==========================================\n");

	while ((n = fread(buff, 1, 8, inf)) > 0) {	    /* АХТУНГ, не мутится проверка на размер сорс файла */
		i++;
		if (n == 8) {
			f(ctx, buff);
			n = fwrite(buff, 1, 8, of);
			if (n != 8)
				return 4;			    //if fwrite error			4
		} else {
			memset(((char*)buff) + n, ' ', 8 - n);
			f(ctx, buff);
			n = fwrite(buff, 1, 8, of);
			if (n != 8)
				return 4;
		}
	}
	
	printf("%d\n", i);
	//blowfish_view(ctx);
	
	fclose(inf);
	fclose(of);
	blowfish_context_free(&ctx);
	return 0;
} 
