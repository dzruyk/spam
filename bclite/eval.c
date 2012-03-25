#include <stdlib.h>

#include "eval.h"
#include "helper_funcs.h"
#include "lex.h"
#include "macros.h"

eval_t *
eval_id_new(id_table_item_t *item)
{
	eval_t *res;

	res = malloc_or_die(sizeof(*res));
	res->type = EVAL_ID;
	
	res->item = item;
	
	return res;
}

eval_t *
eval_num_new(int value)
{
	eval_t *res;
	
	res = malloc_or_die(sizeof(*res));
	res->type = EVAL_NUM;
	res->value = value;

	return res;
}

void
eval_id_free(eval_t *eval)
{
	free(eval);
}

void
eval_num_free(eval_t *eval)
{
	free(eval);
}

void
eval_free(eval_t *eval)
{
	return_if_fail(eval);
	
	switch(eval->type) {
	case EVAL_NUM:
		eval_num_free(eval);
		break;
	case EVAL_ID:
		eval_id_free(eval);
		break;
	}
}


//FIXME: WRITE_ME
eval_t *
eval_process(eval_t *left, eval_t *right, tok_t opcode)
{
	eval_t *ev;
	int l, r, res;

	
	l = eval_get_val(left);
	r = eval_get_val(right);

	switch(opcode) {
	case TOK_MUL:
		res = l * r;
		break;
	case TOK_DIV:
		res = l / r;
		break;
	case TOK_PLUS:
		res = l + r;
		break;
	case TOK_MINUS:
		res = l - r;
		break;
	case TOK_AS:
		if (left->type == EVAL_NUM) {
			print_warn("ERROR: assignment to number\n");
			break;
		} else {
			res = left->item->value = eval_get_val(right);
			break;
		}
	default:
		print_warn_and_die("unsupported tock recognised when eval\n");
	}
	ev = eval_num_new(res);

	eval_free(left);
	eval_free(right);
	return ev;
}

int
eval_get_val(eval_t *eval)
{
	if (eval == NULL)
		print_warn_and_die("INTERNAL ERROR: cant get eval bcs its NULL\n");

	switch(eval->type) {
	case EVAL_NUM:
		return eval->value;
	case EVAL_ID:
		return eval->item->value;
	default:
		print_warn_and_die("INTERNAL ERROR: cant get value\n");
		return 0;
	}
	
}

