#ifndef __LEX_H__
#define __LEX_H__

#include "id_table.h"

typedef enum {
	TOK_ID,
	TOK_NUM,
	TOK_AS,
	TOK_RELOP,
	TOK_LPAR,
	TOK_RPAR,
	TOK_PLUS,
	TOK_MINUS,
	TOK_MUL,
	TOK_DIV,
	TOK_EOL,
	TOK_UNKNOWN,
} tok_t;


typedef enum {
	RELOP_GR,
	RELOP_LO,
	RELOP_GE,
	RELOP_LE,
} relop_t;

struct lex_item {
	int id;
	union {
		int num;
		id_table_item_t *item;
		int op;
	};
};

tok_t get_next_token();

#endif

