#ifndef __ID_TABLE_H__
#define __ID_TABLE_H__

#include "array.h"
#include "common.h"
#include "function.h"

typedef enum {
	ID_NUM,
	ID_ARR,
	ID_FUNC,
	ID_UNKNOWN,
} id_item_t;

typedef struct {
	id_item_t id;
	char *name;
	union {
		int value;
		function_t *func;
		arr_t *arr;
	};
} id_table_item_t;

void id_tables_init();

ret_t id_table_insert(id_table_item_t *item);

id_table_item_t *id_table_lookup(char *name);

void id_tables_destroy();

#endif

