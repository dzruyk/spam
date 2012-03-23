#include <stdio.h>

#include "helper_funcs.h"
#include "lex.h"
#include "macros.h"
#include "stack.h"
#include "syn_tree.h"
#include "traverse.h"

typedef void (*traverse_cb)(syn_tree_t *tree);

static void
traverse_num(syn_tree_t *tree)
{
	int val;
	return_if_fail(NULL != tree);
	
	val = ((syn_tree_num_t*)tree)->num;
	
	stack_push(val);
}


static void
traverse_id(syn_tree_t *tree)
{
	id_table_item_t *item;
	return_if_fail(NULL != tree);

	item = ((syn_tree_id_t*)tree)->item;
	
	stack_push(item->value);
}

static void
traverse_expr(syn_tree_t *tree)
{
	syn_tree_op_t *optree;
	int left, right, res;

	return_if_fail(NULL != tree);

	optree = (syn_tree_op_t*)tree;
	traverse(SYN_TREE(optree)->left);	
	traverse(SYN_TREE(optree)->right);

	right = stack_pop();
	left = stack_pop();
	
	switch(optree->opcode) {
	case TOK_MUL:
		res = left * right;
		break;
	case TOK_DIV:
		res = left / right;
		break;
	case TOK_PLUS:
		res = left + right;
		break;
	case TOK_MINUS:
		res = left - right;
		break;
	default:
		print_warn_and_die("INTERNAL_EROR: can't traverse this operation\n");
	}

	stack_push(res);
}


//FIXME: now I can't assign bcs stack_pop returns just int(
//now I just pop left operand
static void
traverse_as(syn_tree_t *tree)
{
	syn_tree_as_t *astree;
	int res;

	return_if_fail(NULL != tree);
	
	astree = (syn_tree_as_t*)tree;	
	traverse(SYN_TREE(astree)->left);	
	traverse(SYN_TREE(astree)->right);
	
	res = stack_pop();
	stack_pop();
	stack_push(res);
}

struct {
	syn_tree_node_t node;
	traverse_cb callback;
} node_type [] = {
	{SYN_TREE_EXPR, traverse_expr},
	{SYN_TREE_ID, traverse_id},
	{SYN_TREE_NUM, traverse_num},
	{SYN_TREE_AS, traverse_as},
	{SYN_TREE_UNKNOWN, NULL},
};

void
traverse(syn_tree_t *tree)
{
	int i;
	return_if_fail(NULL != tree);
	for (i = 0; SYN_TREE_UNKNOWN != node_type[i].node; i++)
		if (node_type[i].node == tree->type) {
			node_type[i].callback(tree);
			return;
		}
}

void
print_result()
{
	int val;
	val = stack_pop();

	printf("%d\n", val);
}

