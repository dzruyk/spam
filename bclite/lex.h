#ifndef __LEX_H__
#define __LEX_H__

#include "common.h"
#include "id_table.h"
#include "macros.h"

typedef enum {
	TOK_KEYWORD,
	TOK_ID,
	TOK_NUM,
	
	TOK_AS,
	TOK_NOT,
	
	TOK_EQ,
	TOK_NEQ,
	TOK_GR,
	TOK_LO,
	TOK_GE,
	TOK_LE,
	TOK_L_AND,
	TOK_L_OR,
	
	TOK_B_AND,
	TOK_B_OR,

	TOK_LPAR,
	TOK_RPAR,
	TOK_LBRACE,	// {
	TOK_RBRACE,
	TOK_LBRACKET,	// [
	TOK_RBRACKET,
	TOK_COMMA,
	
	TOK_PLUS,
	TOK_MINUS,
	TOK_MUL,
	TOK_DIV,
	
	TOK_EOL,
	TOK_EOF,
	TOK_UNKNOWN,
} tok_t;

struct lex_item {
	tok_t id;
	union {
		int num;
		id_table_item_t *item;
		int op;
	};
};

void tok_next();

inline boolean_t match(const tok_t expect);

tok_t get_next_token();

#endif

