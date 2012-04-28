#ifndef __SYN_NODE_H__
#define __SYN_NODE_H__

#define SYN_NODE(p)((syn_node_t*)p)

#include "id_table.h"


typedef enum {
	OP_AS,
	OP_NOT,
	
	OP_EQ,
	OP_NEQ,
	OP_GR,
	OP_LO,
	OP_GE,
	OP_LE,
	OP_L_AND,
	OP_L_OR,
	
	OP_B_AND,
	OP_B_OR,

	OP_PLUS,
	OP_MINUS,
	OP_MUL,
	OP_DIV,

} opcode_t;


typedef enum {
	SYN_NODE_AS,
	SYN_NODE_OP,
	SYN_NODE_ARR,
	SYN_NODE_ID,
	SYN_NODE_NUM,
	SYN_NODE_STUB,
	SYN_NODE_UNKNOWN,
} syn_node_node_t;

struct syn_node;

typedef void (*destructor_t)(struct syn_node *p);

typedef struct syn_node {
	syn_node_node_t type;
	struct syn_node *left;
	struct syn_node *right;
	destructor_t destructor;
} syn_node_t;	

typedef struct {
	syn_node_t tree;
	int opcode;
} syn_node_op_t;

typedef struct {
	syn_node_t tree;
} syn_node_as_t;

typedef struct {
	syn_node_t tree;
	int num;
} syn_node_num_t;

typedef struct {
	syn_node_t tree;
	id_table_item_t *item;
} syn_node_id_t;

typedef struct {
	syn_node_t tree;
	syn_node_t **arr;
	int sz;
} syn_node_arr_t;

typedef struct {
	syn_node_t tree;
} syn_node_stub_t;


syn_node_t *syn_node_num_new(int num);


syn_node_t *syn_node_id_new(id_table_item_t *item);


syn_node_t *syn_node_arr_new(syn_node_t **arr, int sz);

syn_node_t *syn_node_op_new(syn_node_t* left, syn_node_t *right, opcode_t opcode);


syn_node_t *syn_node_as_new(syn_node_t *left, syn_node_t *right);


syn_node_t *syn_node_stub_new();


void syn_node_unref(syn_node_t *tree);

#endif

