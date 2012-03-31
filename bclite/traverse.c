#include <stdio.h>

#include "common.h"
#include "eval.h"
#include "helper_funcs.h"
#include "lex.h"
#include "macros.h"
#include "stack.h"
#include "syn_tree.h"
#include "traverse.h"

static void traverse(syn_tree_t *tree);

typedef void (*traverse_cb)(syn_tree_t *tree);

static int nerrors = 0;

static void
traverse_num(syn_tree_t *tree)
{
	eval_t *ev;
	int val;
	
	val = ((syn_tree_num_t*)tree)->num;
	
	ev = eval_num_new(val);

	stack_push(ev);
}

static void
traverse_id(syn_tree_t *tree)
{
	id_table_item_t *item;
	eval_t *ev;

	item = ((syn_tree_id_t*)tree)->item;
	
	ev = eval_id_new(item);
	stack_push(ev);
}

static void
traverse_op(syn_tree_t *tree)
{
	syn_tree_op_t *optree;
	eval_t *left, *right, *res;

	optree = (syn_tree_op_t*)tree;
	traverse(SYN_TREE(optree)->left);	
	traverse(SYN_TREE(optree)->right);

	if (nerrors != 0)
		return;
	
	right = stack_pop();
	left = stack_pop();
	
	switch(optree->opcode) {
	case TOK_MUL:
	case TOK_DIV:
		res = eval_process(left, right, optree->opcode);
		break;
	case TOK_PLUS:
	case TOK_MINUS:
		res = eval_process(left, right, optree->opcode);
		break;
	case TOK_AS:
		res = eval_process(left, right, optree->opcode);
		break;
	default:
		print_warn_and_die("INTERNAL_EROR: can't traverse this operation\n");
	}
	if (res == NULL) {
		nerrors++;
		return;
	}
	
	stack_push(res);
}

static void
traverse_stub(syn_tree_t *tree)
{
	nerrors++;
	return;
}

struct {
	syn_tree_node_t node;
	traverse_cb callback;
} node_type [] = {
	{SYN_TREE_EXPR, traverse_op},
	{SYN_TREE_ID, traverse_id},
	{SYN_TREE_NUM, traverse_num},
	{SYN_TREE_STUB, traverse_stub},
	{SYN_TREE_UNKNOWN, NULL},
};

static void
traverse(syn_tree_t *tree)
{
	int i;
	
	return_if_fail(tree != NULL);

	for (i = 0; SYN_TREE_UNKNOWN != node_type[i].node; i++)
		if (node_type[i].node == tree->type) {
			node_type[i].callback(tree);
			return;
		}
}

//ASKME: I need to unref tree here?
//
ret_t
traverse_prog(syn_tree_t *tree)
{
	if (tree == NULL)
		return ret_ok;
	nerrors = 0;

	traverse(tree);
	
	syn_tree_unref(tree);

	if (nerrors != 0) {
		stack_flush();
		return ret_err;
	}
	return ret_ok;
}


//pop first item of stack and prints it
void
traverse_print_result()
{
	eval_t *tmp;
	int val;

	tmp = (eval_t*)stack_pop();

	if (tmp == NULL)
		return;

	val = eval_get_val(tmp);
	
	printf("%d\n", val);

	eval_free(tmp);
}

