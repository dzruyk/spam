#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper_funcs.h"
#include "macros.h"
#include "syn_tree.h"

static void
syn_node_free(syn_node_t *tree)
{
	syn_node_arr_t *arr;

	int i, n;

	return_if_fail(tree != NULL);
	
	switch (tree->type) {
	case SYN_NODE_NUM:
	case SYN_NODE_ID:
		break;
	case SYN_NODE_ARR:
		//CHECK_ME
		arr = (syn_node_arr_t *)tree;
		n = arr->sz;
		for (i = 0; i < n; i++)
			syn_node_free(arr->arr[i]);
		break;
	case SYN_NODE_OP:
	case SYN_NODE_AS:
	case SYN_NODE_STUB:	
		syn_node_unref(tree->left);
		syn_node_unref(tree->right);
		break;
	default:
		print_warn_and_die("something wrong, no such type\n");
	}

	free(tree);
}

syn_node_t *
syn_node_num_new(int num)
{
	syn_node_num_t *res;
	
	res = malloc_or_die(sizeof(*res));
	memset(res, 0, sizeof(*res));

	SYN_NODE(res)->type = SYN_NODE_NUM;
	SYN_NODE(res)->destructor = syn_node_free;
	
	res->num = num;
	
	return SYN_NODE(res);
}

syn_node_t *
syn_node_id_new(id_table_item_t *item)
{
	syn_node_id_t *res;
	
	res = malloc_or_die(sizeof(*res));
	memset(res, 0, sizeof(*res));

	SYN_NODE(res)->type = SYN_NODE_ID;
	SYN_NODE(res)->destructor = syn_node_free;

	res->item = item;

	return SYN_NODE(res);
}


syn_node_t *
syn_node_arr_new(syn_node_t **arr, int sz)
{
	syn_node_arr_t *res;

	res = malloc_or_die(sizeof(*res));
	
	SYN_NODE(res)->type = SYN_NODE_ARR;
	SYN_NODE(res)->destructor = syn_node_free;

	res->arr = arr;
	res->sz = sz;

	return SYN_NODE(res);
}


syn_node_t *
syn_node_op_new(syn_node_t *left, syn_node_t *right, opcode_t opcode)
{
	syn_node_op_t *res;
	
	res = malloc_or_die(sizeof(*res));
	
	SYN_NODE(res)->type = SYN_NODE_OP; 
	SYN_NODE(res)->left = left;
	SYN_NODE(res)->right = right;
	SYN_NODE(res)->destructor = syn_node_free;

	res->opcode = opcode;

	return SYN_NODE(res);
}

syn_node_t *
syn_node_as_new(syn_node_t *left, syn_node_t *right)
{
	syn_node_as_t *res;
	
	res = malloc_or_die(sizeof(*res));
	
	SYN_NODE(res)->type = SYN_NODE_AS; 
	SYN_NODE(res)->left = left;
	SYN_NODE(res)->right = right;
	SYN_NODE(res)->destructor = syn_node_free;

	return SYN_NODE(res);
}

syn_node_t *
syn_node_stub_new()
{
	syn_node_stub_t *res;
	
	res = malloc_or_die(sizeof(*res));

	memset(res, 0, sizeof(*res));
	SYN_NODE(res)->type = SYN_NODE_STUB;
	SYN_NODE(res)->destructor = syn_node_free;

	return SYN_NODE(res);
}

void
syn_node_unref(syn_node_t *tree)
{
	if (tree != NULL)
		tree->destructor(tree);
}

