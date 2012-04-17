#ifndef __EVAL_H__
#define __EVAL_H__

#include "id_table.h"
#include "lex.h"

typedef enum {
	EVAL_NUM,
	EVAL_ID,
	EVAL_FUNC,
	EVAL_ARR,
} eval_type_t;


typedef struct {
	eval_type_t type;
	union {
		int value;
		id_table_item_t *item;
	};
} eval_t;

eval_t *eval_id_new();

eval_t *eval_num_new();

void eval_free(eval_t *eval);

eval_t *eval_process_op(eval_t *left, eval_t *right, tok_t opcode);

eval_t *eval_process_bool(eval_t *left, eval_t *right, tok_t opcode);

eval_t * eval_assign(eval_t *left, eval_t *right);

ret_t eval_get_val(int *res, eval_t *eval);
#endif

