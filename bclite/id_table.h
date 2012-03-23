#ifndef _ID_TABLE_H_
#define _ID_TABLE_H_

#include "common.h"

typedef struct {
	char *name;
	int value;
} id_table_item_t;

void id_table_create();

ret_t id_table_insert(id_table_item_t *item);

id_table_item_t *id_table_lookup(char *name);

void id_table_destroy();

#endif

