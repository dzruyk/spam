#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "id_table.h"
#include "helper_funcs.h"
#include "lex.h"

extern struct hash_table *identifier;

struct lex_item lex_item;
struct lex_item lex_item_prev;
tok_t current_tok;

static void
update_prev_token()
{
	lex_item_prev.id = lex_item.id;
	switch(lex_item.id) {
	case TOK_ID:
		lex_item_prev.item = lex_item.item;
		break;
	case TOK_NUM:
		lex_item_prev.num = lex_item.num;
		break;
	default:
		lex_item_prev.op = lex_item.op;
	}
}

void
tok_next()
{
	update_prev_token();
	current_tok = get_next_token();
}

inline boolean_t
match(const tok_t expect)
{
	if (current_tok == expect) {
		tok_next();
		return TRUE;
	}

	return FALSE;
}


tok_t
get_next_token()
{
	static char peek = ' ';

	for (; peek == ' ' || peek == '\t';)
		peek = fgetc(stdin);
	
	if (isdigit(peek)) {
		int num = 0;
		do {
			num = num * 10 + peek - '0';
			peek = fgetc(stdin);
		} while (isdigit(peek));
		
		lex_item.id = TOK_NUM;
		lex_item.num = num;
		return TOK_NUM;
	}
	if (isalpha(peek)) {
		id_table_item_t *nitem;
		char *s = NULL;
		char *tmp;
		int used, len;
		
		len = used = 0;
		do {
			if (used >= len - 1) {
				len += 64;
				if ((tmp = realloc(s, len)) == NULL)
					print_warn_and_die("realloc_err");
				s = tmp;
			}
			s[used++] = peek;
			peek = fgetc(stdin);
		} while(isdigit(peek) || isalpha(peek));
		//FIXME: now we cant have id with _
		s[used++] = '\0';
		if ((tmp = realloc(s, used)) == NULL)
			print_warn_and_die("realloc_err");
		s = tmp;

		if ((nitem = id_table_lookup(s)) == NULL) {
			nitem = malloc_or_die(sizeof(*nitem));
			nitem->value = 0;
			nitem->name = s;
			id_table_insert(nitem);
		} else {
			free(s);
		}

		lex_item.id = TOK_ID;
		lex_item.item = nitem;
		
		return TOK_ID;
	}

	switch (peek) {
	case '=':
		peek = ' ';
		lex_item.id = TOK_AS;
		lex_item.op = TOK_AS;
		return TOK_AS;
	case '<':
		lex_item.id = TOK_RELOP;
		peek = fgetc(stdin);
		if (peek == '=')
			lex_item.op = RELOP_LE;
		else
			lex_item.op = RELOP_LO;
		return TOK_RELOP;
	case '>':
		lex_item.id = TOK_RELOP;
		peek = fgetc(stdin);
		if (peek == '=')
			lex_item.op = RELOP_GE;
		else
			lex_item.op = RELOP_GR;
		return TOK_RELOP;
	case '+':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_PLUS;
		return TOK_PLUS;
	case '-':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_MINUS;
		return TOK_MINUS;
	case '(':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_LPAR;
		return TOK_LPAR;
	case ')':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_RPAR;
		return TOK_RPAR;
	case '*':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_MUL;
		return TOK_MUL;
	case '/':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_DIV;
		return TOK_DIV;
	case '\n':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_EOL;
		return TOK_EOL;
	case EOF:
		peek = ' ';
		lex_item.id = lex_item.op = TOK_EOF;
		return TOK_EOF;
	}
	
	peek = ' ';
	lex_item.id = lex_item.op = TOK_UNKNOWN;

	return TOK_UNKNOWN;
}

