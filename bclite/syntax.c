#include <stdio.h>

#include "common.h"
#include "helper_funcs.h"
#include "macros.h"
#include "syntax.h"
#include "lex.h"

extern struct lex_item lex_item;
static struct lex_item lex_item_prev;

static tok_t current_tok;

static int nerrors;

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

static void
tok_next()
{
	update_prev_token();
	current_tok = get_next_token();
}

inline static boolean_t
match(tok_t expect)
{
	if (current_tok == expect) {
		tok_next();
		return TRUE;
	}

	return FALSE;
}

syn_tree_t *
program_start()
{
	syn_tree_t *tree;
	nerrors = 0;
	
	tree = NULL;
	tok_next();


	while(TOK_EOL != current_tok && TOK_UNKNOWN != current_tok)
		tree = statesment();
	
	return tree;
}


syn_tree_t *
statesment()
{
	syn_tree_t *right, *result;

	result = expr();
	
	if (TOK_AS == current_tok) {
		tok_next();
		right = statesment();
		result = syn_tree_as_new(result, right);
	}

	return result;
}


syn_tree_t *
expr()
{
	syn_tree_t *tree;

	tree = term();

	if (NULL == tree)
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
		if (NULL == right) {
			print_warn_and_die("uncomplited expression\n");
			return NULL;
		}

		result = syn_tree_op_new(result, right, op);
	}

}

syn_tree_t *
term()
{
	syn_tree_t *tree;

	tree = factor();
	if (NULL == tree) 
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
		if (NULL == right) {
			print_warn_and_die("uncomplited expression\n");
			return NULL;
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
		if (FALSE == match(TOK_RPAR)) {
			print_warn("right parenthesis missed\n");
			nerrors++;
			return NULL;
		}
		return stat;
	} else if (TOK_EOL == current_tok)
		return NULL;
	else if (TOK_UNKNOWN == current_tok)
		return NULL;

	print_warn_and_die("unsupported expression\n");
	return NULL;
}

