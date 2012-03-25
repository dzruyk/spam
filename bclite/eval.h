#ifndef _EVAL_H_
#define _EVAL_H_

#include "id_table.h"

typedef enum {
	EVAL_NUM,
	EVAL_ID,
} eval_type_t;


typedef struct {
	int type;
	union {
		int value;
		id_table_item_t *item;
	};
} eval_t;

eval_t *eval_id_new();

eval_t *eval_num_new();

void eval_id_free();

void eval_num_free();

eval_t *eval_process();

int eval_get_val(eval_t *eval);
#endif

