#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "eval.h"
#include "macros.h"

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
	//warning, may be memory leak
	case EVAL_ARR:
		free(eval);
		break;
	default:
		print_warn_and_die("WIP\n");
	}
}

eval_t *
eval_process_op(eval_t *left, eval_t *right, opcode_t opcode)
{
	eval_t *ev;
	int l, r, res;
	
	//FIXME
	if (left->type != EVAL_NUM)
		return NULL;
	if (right->type != EVAL_NUM)
		return NULL;
	
	l = left->value;
	r = right->value;

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
eval_print_val(eval_t *eval)
{
	int value;

	if (eval == NULL)
		return ret_err;	

	switch(eval->type) {
	case EVAL_NUM:
		value = eval->value;
		printf("%d\n", value);
		break;
	case EVAL_ARR:
		arr_print(eval->arr);	
		break;
	default:
		print_warn_and_die("INTERNAL ERROR: cant get value\n");
	}

	return ret_ok; 
}

