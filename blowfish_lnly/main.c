#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "blowfish.h"

static uint32_t newP[] = {
	0xFFFFFFFF, 0x85A308D3, 0x13198A2E, 0x03707344,
        0xA4093822, 0x299F31D0, 0x082EFA98, 0xEC4E6C89,
        0x452821E6, 0x38D01377, 0xBE5466CF, 0x34E90C6C,
        0xC0AC29B7, 0xC97C50DD, 0x3F84D5B5, 0xB5470917,
        0x9216D5D9, 0x8979FB1B
};
static uint32_t key[] = {
	0x12345678, 0xFFFFFFFF, 0xAAAABBBB
};

static uint32_t var[]= { 0xdeadbeef, 0xabcdef};

int
main(int argc, char *argv[])
{
	int i;
	struct blowfish_context *ctx;
	uint32_t tmp[2];

	ctx = blowfish_context_new();
	
	blowfish_set_p(ctx, newP);

	blowfish_set_key(ctx, (unsigned char*) "TESTKEY", 7);
	
	printf("\nbefore encr = ");
	for (i = 0; i < 8; i++)
		printf("%x.",((unsigned char*) var)[i]);
	
	memcpy(tmp, var, sizeof(var));
	
	blowfish_encrypt(ctx, (uint32_t*) var);
	
	printf("\nafter encr = ");
	for (i = 0; i < 8; i++)
		printf("%x.",((unsigned char*) var)[i]);
	
	blowfish_decrypt(ctx,(uint32_t*) var);

	printf("\nafter decr = ");
	for (i = 0; i < 8; i++)
		printf("%x.",((unsigned char*) var)[i]);	
	printf("\n");
	
	if (strncmp((char*)tmp, (char*)var, sizeof(var)))
		printf("%s%s", "\n===================================",
		    "\ntest failed!!!!!!!!!!\n\n===================================\n");
	else
		printf("%s%s", "\n===================================",
		    "\nTEST PASSED\n\n===================================\n");
	return 0;
}

