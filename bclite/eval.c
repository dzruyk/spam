#include <stdlib.h>

#include "common.h"
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

eval_t *
eval_arr_new(arr_t *arr)
{
	eval_t *res;
	
	res = malloc_or_die(sizeof(*res));
	res->type = EVAL_ARR;
	res->arr = arr;

	return res;
}

void
eval_free(eval_t *eval)
{
	return_if_fail(eval);
	
	switch(eval->type) {
	case EVAL_NUM:
		free(eval);
		break;
	case EVAL_ID:
		free(eval);
		break;
	default:
		print_warn_and_die("WIP\n");
	}
}

//WARNING: now i return modified left operand
//
eval_t *
eval_assign(eval_t *left, eval_t *right)
{
	int res;

	if (left->type == EVAL_NUM) {
		print_warn("ERROR: assignment to number\n");
		return NULL;	
	} else if (left->type == EVAL_ID) {
		eval_get_val(&res, right);
		left->item->id = ID_NUM;
		left->item->value = res;
	} else {
		print_warn("eval_assign: something wrong\n");
		return NULL;
	}
	
	return left;
}

eval_t *
eval_process_op(eval_t *left, eval_t *right, opcode_t opcode)
{
	eval_t *ev;
	int l, r, res;
	
	if (eval_get_val(&l, left) != ret_ok)
		return NULL;
	if (eval_get_val(&r, right) != ret_ok)
		return NULL;

	switch(opcode) {
	case OP_MUL:
		res = l * r;
		break;
	case OP_DIV:
		if (r == 0) {
			print_warn("divide by zero\n");
			return NULL;
		}
		res = l / r;
		break;
	case OP_PLUS:
		res = l + r;
		break;
	case OP_MINUS:
		res = l - r;
		break;
	case OP_EQ:
		res = (l == r);
		break;
	case OP_NEQ:
		res = (l != r);
		break;
	case OP_GR:
		res = (l > r);
		break;
	case OP_LO:
		res = (l < r);
		break;
	case OP_GE:
		res = (l >= r);
		break;
	case OP_LE:
		res = (l <= r);
		break;
	case OP_L_AND:
		res = (l && r);
		break;
	case OP_L_OR:
		res = (l || r);
		break;
	default:
		print_warn_and_die("unsupported tock recognised when eval\n");
	}
	ev = eval_num_new(res);

	return ev;
}

ret_t
eval_get_val(int *res, eval_t *eval)
{
	int value;

	if (eval == NULL)
		return ret_err;	

	switch(eval->type) {
	case EVAL_NUM:
		value = eval->value;
		break;
	case EVAL_ID:
		if (eval->item->id == ID_UNKNOWN) {
			print_warn("symb %s undefined\n", eval->item->name);
			return ret_err;
		} else if (eval->item->id == ID_NUM)
			value = eval->item->value;
		break;
	case EVAL_ARR:
		print_warn_and_die("WIP\n");
	
	default:
		print_warn_and_die("INTERNAL ERROR: cant get value\n");
	}

	*res = value;
	
	return ret_ok; 
}

