#ifndef _SYN_TREE_H_
#define _SYN_TREE_H_

#define SYN_TREE(p)((syn_tree_t*)p)

#include "id_table.h"

typedef enum {
	SYN_TREE_EXPR,
	SYN_TREE_ID,
	SYN_TREE_NUM,
	SYN_TREE_AS,
	SYN_TREE_UNKNOWN,
} syn_tree_node_t;

struct syn_tree;

typedef void (*destructor_t)(struct syn_tree *p);

typedef struct syn_tree {
	int type;
	struct syn_tree *left;
	struct syn_tree *right;
	destructor_t destructor;
} syn_tree_t;	


typedef struct {
	syn_tree_t tree;
	int opcode;
} syn_tree_op_t;

typedef struct {
	syn_tree_t tree;
	int num;
} syn_tree_num_t;

typedef struct {
	syn_tree_t tree;
	id_table_item_t *item;
} syn_tree_id_t;

typedef struct {
	syn_tree_t tree;
} syn_tree_as_t;

syn_tree_t *syn_tree_num_new(int num);

syn_tree_t *syn_tree_id_new(id_table_item_t *item);

syn_tree_t *syn_tree_op_new(syn_tree_t* left, syn_tree_t *right,int opcode);

void syn_tree_unref(syn_tree_t *tree);

#endif
