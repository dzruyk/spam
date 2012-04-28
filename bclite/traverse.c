#include <stdio.h>

#include "array.h"
#include "common.h"
#include "eval.h"
#include "helper_funcs.h"
#include "id_table.h"
#include "lex.h"
#include "macros.h"
#include "stack.h"

#include "traverse.h"

static void traverse(syn_node_t *tree);

typedef void (*traverse_cb)(syn_node_t *tree);

static int nerrors = 0;

static void
traverse_num(syn_node_t *tree)
{
	eval_t *ev;
	int val;
	
	val = ((syn_node_num_t *)tree)->num;
	
	ev = eval_num_new(val);

	stack_push(ev);
}

static void
traverse_id(syn_node_t *tree)
{
	eval_t *ev;
	id_table_item_t *item;

	item = ((syn_node_id_t *)tree)->item;
	
	ev = eval_id_new(item);
	stack_push(ev);
}

static void
traverse_arr(syn_node_t *tree)
{
	eval_t *ev;
	arr_t *arr;
	syn_node_t **synarr;
	int i, sz, res;
	
	synarr = ((syn_node_arr_t *)tree)->arr;
	sz = ((syn_node_arr_t *)tree)->sz;

	//size of int
	arr = arr_new(sz, sizeof(int));

	for (i = 0; i < sz; i++) {
		traverse(synarr[i]);
		if ((ev = stack_pop()) == NULL)
			print_warn_and_die("unexpected error\n");
		
		if (eval_get_val(&res, ev) != ret_ok)
			print_warn_and_die("unexpected error\n");

		if (arr_set_item(arr, i, res) != ret_ok)
			print_warn_and_die("cant set item\n");
	}
	
	ev = eval_arr_new(arr);
	stack_push(ev);
}

static void
traverse_as(syn_node_t *tree)
{
	syn_node_as_t *optree;
	eval_t *left, *right, *res;

	optree = (syn_node_as_t *)tree;
	traverse(SYN_NODE(optree)->left);	
	traverse(SYN_NODE(optree)->right);

	if (nerrors != 0)
		return;
	
	right = stack_pop();
	left = stack_pop();
	
	res = eval_assign(left, right);
	if (res == NULL) {
		nerrors++;
		eval_free(left);
		eval_free(right);
		return;
	}
	
	eval_free(right);

	stack_push(res);
}

static void
traverse_op(syn_node_t *tree)
{
	syn_node_op_t *optree;
	eval_t *left, *right, *res;

	optree = (syn_node_op_t *)tree;
	traverse(SYN_NODE(optree)->left);	
	traverse(SYN_NODE(optree)->right);

	if (nerrors != 0)
		return;
	
	right = stack_pop();
	left = stack_pop();
	
	res = eval_process_op(left, right, optree->opcode);
	eval_free(left);
	eval_free(right);
	
	if (res == NULL) {
		nerrors++;
		return;
	}
	
	stack_push(res);
}

static void
traverse_stub(syn_node_t *tree)
{
	nerrors++;
	return;
}

struct {
	syn_node_node_t node;
	traverse_cb callback;
} node_type [] = {
	{SYN_NODE_AS, traverse_as},
	{SYN_NODE_OP, traverse_op},
	{SYN_NODE_ARR, traverse_arr},
	{SYN_NODE_ID, traverse_id},
	{SYN_NODE_NUM, traverse_num},
	{SYN_NODE_STUB, traverse_stub},
	{SYN_NODE_UNKNOWN, NULL},
};

static void
traverse(syn_node_t *tree)
{
	int i;
	return_if_fail(tree != NULL);

	for (i = 0; node_type[i].node != SYN_NODE_UNKNOWN; i++)
		if (node_type[i].node == tree->type) {
			node_type[i].callback(tree);
			return;
		}
}

//ASKME: I need to unref tree here?
//
ret_t
traverse_prog(syn_node_t *tree)
{
	if (tree == NULL)
		return ret_ok;
	
	nerrors = 0;

	traverse(tree);
	
	syn_node_unref(tree);

	if (nerrors != 0) {
		stack_flush((stack_item_free_t )eval_free);
		return ret_err;
	}
	return ret_ok;
}

void
traverse_print_result()
{
	eval_t *tmp;
	int val;

	while ((tmp = (eval_t *)stack_pop()) != NULL) {
	
		if (eval_get_val(&val, tmp) != ret_ok) {
			eval_free(tmp);
			break;
		}

		printf("%d\n", val);

		eval_free(tmp);
	}
}

