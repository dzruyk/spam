#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper_funcs.h"
#include "macros.h"
#include "syn_tree.h"


static void
syn_tree_num_free(syn_tree_t *tree)
{
	return_if_fail(tree != NULL);
	free(tree);
}

static void
syn_tree_id_free(syn_tree_t *tree)
{
	return_if_fail(tree != NULL);
	free(tree);
}

static void
syn_tree_op_free(syn_tree_t *tree)
{	
	return_if_fail(tree != NULL);
	
	syn_tree_unref(tree->left);
	syn_tree_unref(tree->right);
	
	free(tree);
}

static void
syn_tree_as_free(syn_tree_t *tree)
{
	return_if_fail(tree != NULL);

	syn_tree_unref(tree->left);
	syn_tree_unref(tree->right);
	free(tree);
}

syn_tree_t *
syn_tree_num_new(int num)
{
	syn_tree_num_t *res;
	
	res = malloc_or_die(sizeof(*res));
	memset(res, 0, sizeof(*res));

	SYN_TREE(res)->type = SYN_TREE_NUM;
	SYN_TREE(res)->destructor = syn_tree_num_free;
	
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
	SYN_TREE(res)->destructor = syn_tree_id_free;

	res->item = item;

	return SYN_TREE(res);
}

syn_tree_t *
syn_tree_op_new(syn_tree_t *left, syn_tree_t *right,int opcode)
{
	syn_tree_op_t *res;
	
	res = malloc_or_die(sizeof(*res));
	
	SYN_TREE(res)->type = SYN_TREE_EXPR; 
	SYN_TREE(res)->left = left;
	SYN_TREE(res)->right = right;
	SYN_TREE(res)->destructor = syn_tree_op_free;

	res->opcode = opcode;

	return SYN_TREE(res);
}

syn_tree_t *
syn_tree_as_new(syn_tree_t *left, syn_tree_t *right)
{
	syn_tree_op_t *res;

	res = malloc_or_die(sizeof(*res));

	SYN_TREE(res)->left = left;
	SYN_TREE(res)->right = right;
	SYN_TREE(res)->type = SYN_TREE_AS;
	
	SYN_TREE(res)->destructor = syn_tree_as_free;

	return SYN_TREE(res);
}

void
syn_tree_unref(syn_tree_t *tree)
{
	return_if_fail(tree != NULL);
	tree->destructor(tree);
}

