#include <stdio.h>

#include "syn_tree.h"
#include "common.h"
#include "helper_funcs.h"
#include "keyword.h"
#include "macros.h"
#include "syntax.h"
#include "lex.h"

static syn_node_t *statesment();

static syn_node_t *logic_disj();
static syn_node_t *logic_conj();
static syn_node_t *equity();
static syn_node_t *rel_op();

static syn_node_t *expr();
static syn_node_t *expr_rest();
static syn_node_t *term();
static syn_node_t *term_rest();
static syn_node_t *factor();

static syn_node_t *identifier();

static syn_node_t *call_function();
static syn_node_t *access_array();
static syn_node_t *array_init();

extern struct lex_item lex_item;
extern struct lex_item lex_item_prev;
extern tok_t current_tok;


int syntax_is_eof = 0;

static int nerrors;

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

static void
tok_next()
{
	update_prev_token();
	current_tok = get_next_token();
}

static inline boolean_t
match(const tok_t expect)
{
	if (current_tok == expect) {
		tok_next();
		return TRUE;
	}

	return FALSE;
}

ret_t
program_start(syn_node_t **tree)
{
	nerrors = 0;
	*tree = NULL;
	
	tok_next();

	*tree = statesment();
	
	if (nerrors != 0) {
		//now we must flush tree
		if (*tree != NULL) {
			syn_node_unref(*tree);
			*tree = NULL;
		}
		return ret_err;
	}

	return ret_ok;
}


//FIXME: REWRITEME!
//
static syn_node_t *
statesment()
{
	syn_node_t *right, *result;

	result = logic_disj();

	if (result == NULL)
		return NULL;
	
	if (match(TOK_AS) == FALSE)
		return result;
	

	//ASSIGNMENT
	
	//array
	if (match(TOK_LBRACE)) {
		right = array_init();
		
		if (match(TOK_RBRACE) == FALSE) {
			print_warn("right bracket missed\n");
			nerrors++;
		}

		return syn_node_as_new(result, right);
	}

	right = statesment();
	if (right == NULL) {
		nerrors++;
		print_warn("uncomplited as expression\n");
		right = syn_node_stub_new();
	}

	return syn_node_as_new(result, right);
}

static syn_node_t *
logic_disj()
{
	syn_node_t *right, *result;

	result = logic_conj();

	if (result == NULL)
		return NULL;
	
	while (TRUE) {
		if (match(TOK_L_OR) == FALSE)
			return result;

		right = logic_conj();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited eq expression\n");
			right = syn_node_stub_new();
		}
		result = syn_node_op_new(result, right, OP_L_OR);
	}
	return result;

}

static syn_node_t *
logic_conj()
{
	syn_node_t *right, *result;

	result = equity();

	if (result == NULL)
		return NULL;
	
	while (TRUE) {
		if(match(TOK_L_AND) == FALSE)
			return result;
		
		right = equity();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited eq expression\n");
			right = syn_node_stub_new();
		}
		result = syn_node_op_new(result, right, OP_L_AND);
	}
	return result;
}

static syn_node_t *
equity()
{
	syn_node_t *right, *result;
	opcode_t op;

	result = rel_op();

	if (result == NULL)
		return NULL;
	
	while (TRUE) {
		switch (current_tok) {
		case TOK_EQ:
			op = OP_EQ;
			break;
		case TOK_NEQ:
			op = OP_NEQ;
			break;
		default:
			return result;
		}
		tok_next();

		right = rel_op();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited eq expression\n");
			right = syn_node_stub_new();
		}
		result = syn_node_op_new(result, right, op);
	}
	return result;

}

static syn_node_t *
rel_op()
{
	syn_node_t *right, *result;
	int op;

	result = expr();

	if (result == NULL)
		return NULL;
	
	while (TRUE) {
		switch (current_tok) {
		case TOK_GR:
			op = OP_GR;
			break;
		case TOK_GE:
			op = OP_GE;
			break;
		case TOK_LO:
			op = OP_LO;
			break;
		case TOK_LE:
			op = OP_LE;
			break;
		default:
			return result;
		}
		tok_next();

		right = expr();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited rel expression\n");
			right = syn_node_stub_new();
		}
		result = syn_node_op_new(result, right, op);
	}
}


static syn_node_t *
expr()
{
	syn_node_t *tree;

	tree = term();

	if (tree == NULL)
		return NULL;
	
	return expr_rest(tree);
}

syn_node_t *
expr_rest(syn_node_t *left)
{
	syn_node_t *right, *result;
	int op;

	result = left;

	while(TRUE) {
		switch (current_tok) {
		case TOK_PLUS:
			op = OP_PLUS;
			break;
		case TOK_MINUS:
			op = OP_MINUS;
			break;
		default:
			return result;
		}
		tok_next();

		right = term();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited expr expression\n");
			right = syn_node_stub_new();
		}

		result = syn_node_op_new(result, right, op);
	}
}

static syn_node_t *
term()
{
	syn_node_t *tree;

	tree = factor();
	if (tree == NULL) 
		return NULL;

	return term_rest(tree);
}

static syn_node_t *
term_rest(syn_node_t *left)
{
	syn_node_t *right, *result;
	int op;

	result = left;

	while(TRUE) {
		switch (current_tok) {
		case TOK_MUL:
			op = OP_MUL;
			break;
		case TOK_DIV:
			op = OP_DIV;
			break;
		default:
			return result;
		}
		tok_next();

		right = factor();
		if (right == NULL) {
			nerrors++;
			print_warn("uncomplited term expression\n");
			right = syn_node_stub_new();
		}

		result = syn_node_op_new(result, right, op);
	}
}

// may be need to return syn_node_stub_t when RBRACKET or RPAR missed?

static syn_node_t *
factor()
{
	syn_node_t *stat;
	
	if (match(TOK_ID)) {
		
		return 	identifier();
	
	} else if (match(TOK_NUM)) {
		
		return syn_node_num_new(lex_item_prev.num);

	}  else if (match(TOK_LPAR)) {
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
	
	stat = syn_node_stub_new();

	return stat;
}

static syn_node_t *
identifier()
{
	switch (current_tok) {
	case TOK_LBRACKET:
		return access_array();
	case TOK_LPAR:
		return call_function();
	default:
		return syn_node_id_new(lex_item_prev.item);
	}
}

static syn_node_t *
call_function()
{
	print_warn_and_die("WIP!\n");
}

static syn_node_t *
access_array()
{
	id_table_item_t *item;

	item = lex_item_prev.item;


	print_warn_and_die("WIP!\n");
}

static syn_node_t *
array_init()
{
	syn_node_t *item, **arr;

	int i, len, sz;
	
	arr = NULL;
	len = sz = 0;

	do {
		item = logic_disj();
		if (item == NULL) {
			nerrors++;
			print_warn("uncomplited tuple\n");
			goto error;
		}
		if (len >= sz) {
			syn_node_t **tmp;

			sz += 4 * sizeof (*arr);

			tmp = realloc(arr, sz);
			if (tmp == NULL)
				print_warn_and_die("realloc_err\n");
			arr = tmp;
		}
		
		arr[len++] = item;

	} while (match(TOK_COMMA) != FALSE);

	return syn_node_arr_new(arr, len);

error:
	for (i = 0; i < len; i++)
		syn_node_unref(arr[i]);
	
	free(arr);
	return syn_node_stub_new();
}

