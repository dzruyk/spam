#ifndef _ID_TABLE_H_
#define _ID_TABLE_H_

#include "common.h"

typedef enum {
	ID_NUM,
	ID_FUNC,
	ID_UNKNOWN,
} id_item_t;

typedef struct {
	id_item_t id;
	char *name;
	int value;
} id_table_item_t;

void id_table_create();

ret_t id_table_insert(id_table_item_t *item);

id_table_item_t *id_table_lookup(char *name);

void id_table_destroy();

#endif

