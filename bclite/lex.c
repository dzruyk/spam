#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "keyword.h"
#include "lex.h"

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
		keyword_t kword;
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

		if ((kword = keyword_table_lookup(s)) != KEY_UNKNOWN) {
			free(s);
			lex_item.id = TOK_KEYWORD;
			lex_item.op = kword;
			return TOK_KEYWORD;
		}

		if ((nitem = id_table_lookup(s)) == NULL) {
			nitem = malloc_or_die(sizeof(*nitem));
			nitem->id = ID_UNKNOWN;
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
	//FIXME:
	// mb need flush peek in composite tokens
	switch (peek) {
	case '=':
		peek = fgetc(stdin);
		if (peek == '=') {
			lex_item.id = lex_item.op = TOK_EQ;
			peek = ' ';
			return TOK_EQ;
		}
		lex_item.id = lex_item.op = TOK_AS;
		
		return TOK_AS;
	case '!':
		peek = fgetc(stdin);
		if (peek == '=') {
			peek = ' ';
			lex_item.id = lex_item.op = TOK_NEQ;
			
			return TOK_NEQ;
		}
		lex_item.id = lex_item.op = TOK_NOT;

		return TOK_NOT;
	case '<':
		peek = fgetc(stdin);
		if (peek == '=') {
			peek = ' ';
			lex_item.id = lex_item.op = TOK_LE;
			
			return TOK_LE;
		}
		lex_item.op = TOK_LO;
		
		return TOK_LO;
	case '>':
		peek = fgetc(stdin);
		if (peek == '=') {
			peek = ' ';
			lex_item.id = lex_item.op = TOK_GE;
			
			return TOK_GE;
		}
		lex_item.id = lex_item.op = TOK_GR;
		
		return TOK_GR;
	case '&':
		peek = fgetc(stdin);
		if (peek == '&') {
			peek = ' ';
			lex_item.id = lex_item.op = TOK_L_AND;
			
			return TOK_L_AND;
		}
		lex_item.id = lex_item.op = TOK_B_AND;
		
		return TOK_B_AND;
	case '|':
		peek = fgetc(stdin);
		if (peek == '|') {
			peek = ' ';
			lex_item.id = lex_item.op = TOK_L_OR;
			
			return TOK_L_OR;
		}
		lex_item.id = lex_item.op = TOK_B_OR;
		
		return TOK_B_OR;
	case '(':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_LPAR;
		
		return TOK_LPAR;
	case ')':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_RPAR;
		
		return TOK_RPAR;
	case '[':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_LBRACKET;
		
		return TOK_LBRACKET;
	case ']':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_RBRACKET;
		
		return TOK_RBRACKET;
	case '{':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_LBRACE;
		
		return TOK_LBRACE;
	case '}':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_RBRACE;
		
		return TOK_RBRACE;
	case ',':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_COMMA;

		return TOK_COMMA;
	case '+':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_PLUS;
		
		return TOK_PLUS;
	case '-':
		peek = ' ';
		lex_item.id = lex_item.op = TOK_MINUS;
		
		return TOK_MINUS;
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

