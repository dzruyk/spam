#include <stdio.h>

#include "common.h"
#include "helper_funcs.h"
#include "macros.h"
#include "syntax.h"
#include "lex.h"

extern struct lex_item lex_item;
extern struct lex_item lex_item_prev;
extern tok_t current_tok;


int syntax_is_eof = 0;

static int nerrors;

//WARNING:
//now I not remove nodes after error

ret_t
program_start(syn_tree_t **tree)
{
	nerrors = 0;
	*tree = NULL;
	
	tok_next();

	*tree = statesment();
	
	if (nerrors != 0) {
		//now we must flush tree
		if (*tree != NULL)
			syn_tree_unref(*tree);
			*tree = NULL;
		return ret_err;
	}

	return ret_ok;
}


syn_tree_t *
statesment()
{
	syn_tree_t *right, *result;

	result = expr();

	if (result == NULL)
		return NULL;
	
	if (current_tok == TOK_AS) {
		tok_next();
		right = statesment();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited as expression\n");
			return result;
		}
		result = syn_tree_op_new(result, right, TOK_AS);
	}
	return result;
}


syn_tree_t *
expr()
{
	syn_tree_t *tree;

	tree = term();

	if (tree == NULL)
		return NULL;
	
	return expr_rest(tree);
}

syn_tree_t *
expr_rest(syn_tree_t *left)
{
	syn_tree_t *right, *result;
	int op;

	result = left;

	while(TRUE) {
		switch (current_tok) {
		case TOK_PLUS:
			op = TOK_PLUS;
			break;
		case TOK_MINUS:
			op = TOK_MINUS;
			break;
		default:
			return result;
		}
		tok_next();

		right = term();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited expr expression\n");
			return result;
		}

		result = syn_tree_op_new(result, right, op);
	}
}

syn_tree_t *
term()
{
	syn_tree_t *tree;

	tree = factor();
	if (tree == NULL) 
		return NULL;

	return term_rest(tree);
}

syn_tree_t *
term_rest(syn_tree_t *left)
{
	syn_tree_t *right, *result;
	int op;

	result = left;

	while(TRUE) {
		switch (current_tok) {
		case TOK_MUL:
			op = TOK_MUL;
			break;
		case TOK_DIV:
			op = TOK_DIV;
			break;
		default:
			return result;
		}
		tok_next();

		right = factor();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited term expression\n");
			return result;
		}

		result = syn_tree_op_new(result, right, op);
	}
}

syn_tree_t *
factor()
{
	syn_tree_t *stat;

	if (match(TOK_ID))
		return syn_tree_id_new(lex_item_prev.item);
	else if (match(TOK_NUM))
		return syn_tree_num_new(lex_item_prev.num);
	else if (match(TOK_LPAR)) {
		stat = statesment();
		if (match(TOK_RPAR) == FALSE) {
			print_warn("right parenthesis missed\n");
			nerrors++;
			return stat;
		}
		return stat;
	} else if (current_tok == TOK_EOL) {
		return NULL;
	} else if (current_tok == TOK_EOF) {
		syntax_is_eof = 1;
		return NULL;
	}

	nerrors++;
	print_warn("unsupported token tryed to factor\n");
	tok_next();
	
	return NULL;
}

