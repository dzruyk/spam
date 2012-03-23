#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "id_table.h"
#include "helper_funcs.h"
#include "lex.h"

extern struct hash_table *identifier;

struct lex_item lex_item;

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
				if (NULL == (tmp = realloc(s, len)))
					print_warn_and_die("realloc_err");
				s = tmp;
			}
			s[used++] = peek;
			peek = fgetc(stdin);
		} while(isdigit(peek) || isalpha(peek));
		//FIXME: now we cant have id with _
		s[used++] = '\0';
		if (NULL == (tmp = realloc(s, used)))
			print_warn_and_die("realloc_err");
		s = tmp;

		if (NULL == (nitem = id_table_lookup(s))) {
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
		if ('=' == peek)
			lex_item.op = RELOP_LE;
		else
			lex_item.op = RELOP_LO;
		return TOK_RELOP;
	case '>':
		lex_item.id = TOK_RELOP;
		peek = fgetc(stdin);
		if ('=' == peek)
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
	}
	
	lex_item.id = lex_item.op = TOK_UNKNOWN;
	
	return TOK_UNKNOWN;
}

