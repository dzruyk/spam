#ifndef BLOWFISH_H_
#define BLOWFISH_H_

/* FIXME, FIXME PLEASE!!!! */
//#include <stdint.h>
/* WARNING: NEED TO INCLUDE stdint.h. */

struct blowfish_context;

enum {
	P_SZ = 18,
	NSBOX = 4,
	NELEMS = 256,
	MINKEYSZ = 4,			/* Mininum keysz in bytes */
	MAXKEYSZ = 56,		/*Maximum keysz in bytes */
};

/* Add new context structure or NULL if error. By default feels P and sbox arrays by digits of P. */
struct blowfish_context *blowfish_context_new();

/* Free context structure. */
void blowfish_context_free(struct blowfish_context **ctx);

/* Change P array in context. WARNING: need to control size of newP(18 elems of uint32_t).  */
void blowfish_set_p(struct blowfish_context *ctx, uint32_t *newP);

void blowfish_set_sbox(struct blowfish_context *ctx, uint32_t *sbox);

/* Try to set key, returns -1 if fails. Function dont check size of key. */
int blowfish_set_key(struct blowfish_context *ctx, const unsigned char *key, int sz);


void blowfish_encrypt(struct blowfish_context *ctx, uint32_t *block);


void blowfish_decrypt(struct blowfish_context *ctx, uint32_t *block);


/* ============================================ */
void blowfish_view(struct blowfish_context *ctx);

#endif
