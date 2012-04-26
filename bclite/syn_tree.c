#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper_funcs.h"
#include "macros.h"
#include "syn_tree.h"

static void
syn_tree_free(syn_tree_t *tree)
{
	syn_tree_arr_t *arr;

	int i, n;

	return_if_fail(tree != NULL);
	
	switch (tree->type) {
	case SYN_TREE_NUM:
	case SYN_TREE_ID:
		break;
	case SYN_TREE_ARR:
		//CHECK_ME
		arr = (syn_tree_arr_t *)tree;
		n = arr->sz;
		for (i = 0; i < n; i++)
			syn_tree_free(arr->arr[i]);
		break;
	case SYN_TREE_OP:
	case SYN_TREE_AS:
	case SYN_TREE_STUB:	
		syn_tree_unref(tree->left);
		syn_tree_unref(tree->right);
		break;
	default:
		print_warn_and_die("something wrong, no such type\n");
	}

	free(tree);
}

syn_tree_t *
syn_tree_num_new(int num)
{
	syn_tree_num_t *res;
	
	res = malloc_or_die(sizeof(*res));
	memset(res, 0, sizeof(*res));

	SYN_TREE(res)->type = SYN_TREE_NUM;
	SYN_TREE(res)->destructor = syn_tree_free;
	
	res->num = num;
	
	return SYN_TREE(res);
}

syn_tree_t *
syn_tree_id_new(id_table_item_t *item)
{
	syn_tree_id_t *res;
	
	res = malloc_or_die(sizeof(*res));
	memset(res, 0, sizeof(*res));

	SYN_TREE(res)->type = SYN_TREE_ID;
	SYN_TREE(res)->destructor = syn_tree_free;

	res->item = item;

	return SYN_TREE(res);
}


syn_tree_t *
syn_tree_arr_new(syn_tree_t **arr, int sz)
{
	syn_tree_arr_t *res;

	res = malloc_or_die(sizeof(*res));
	
	SYN_TREE(res)->type = SYN_TREE_ARR;
	SYN_TREE(res)->destructor = syn_tree_free;

	res->arr = arr;
	res->sz = sz;

	return SYN_TREE(res);
}


syn_tree_t *
syn_tree_op_new(syn_tree_t *left, syn_tree_t *right,int opcode)
{
	syn_tree_op_t *res;
	
	res = malloc_or_die(sizeof(*res));
	
	SYN_TREE(res)->type = SYN_TREE_OP; 
	SYN_TREE(res)->left = left;
	SYN_TREE(res)->right = right;
	SYN_TREE(res)->destructor = syn_tree_free;

	res->opcode = opcode;

	return SYN_TREE(res);
}

syn_tree_t *
syn_tree_as_new(syn_tree_t *left, syn_tree_t *right)
{
	syn_tree_as_t *res;
	
	res = malloc_or_die(sizeof(*res));
	
	SYN_TREE(res)->type = SYN_TREE_AS; 
	SYN_TREE(res)->left = left;
	SYN_TREE(res)->right = right;
	SYN_TREE(res)->destructor = syn_tree_free;

	return SYN_TREE(res);
}

syn_tree_t *
syn_tree_stub_new()
{
	syn_tree_stub_t *res;
	
	res = malloc_or_die(sizeof(*res));

	memset(res, 0, sizeof(*res));
	SYN_TREE(res)->type = SYN_TREE_STUB;
	SYN_TREE(res)->destructor = syn_tree_free;

	return SYN_TREE(res);
}

void
syn_tree_unref(syn_tree_t *tree)
{
	if (tree != NULL)
		tree->destructor(tree);
}

