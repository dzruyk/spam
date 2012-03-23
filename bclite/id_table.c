#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "hash.h"
#include "helper_funcs.h"
#include "id_table.h"
#include "lex.h"
#include "macros.h"

#define INITIAL_SZ 32


struct hash_table *identifier;

static int
id_compare(void *a, void *b)
{	
	return strcmp((char*)a, (char*)b);
}

static unsigned long
id_hash_cb(const void *data)
{
	int i, mult, res;
	char *s;
	
	mult = 31;
	res = 0;
	s = (char*)data;

	for (i = 0; i < strlen(data); i++)
		res = res * mult + s[i];
	return res;
}

void
id_table_create()
{
	identifier = hash_table_new(INITIAL_SZ, (hash_callback_t)id_hash_cb,
	    (hash_compare_t)id_compare);
	
	if (NULL == identifier)
		print_warn_and_die("error at identifier table creation\n");
}

ret_t
id_table_insert(id_table_item_t *item)
{
	int res;
	//TODO: запихивать нормальную структуру а не просто строку
	res = hash_table_insert_unique(identifier, item->name, item);
	if (ret_out_of_memory == res)
		print_warn_and_die("error at id table insertion\n");
	else if (ret_entry_exists == res)
		print_warn_and_die("internal error, entry exists\n");

	return ret_ok;
}

id_table_item_t *
id_table_lookup(char *name)
{
	id_table_item_t *res;
	
	if (ret_ok == hash_table_lookup(identifier, name, (void**)&res))
		return res;
	else
		return NULL;
}

static void
id_table_destroy_cb(id_table_item_t *item)
{
	free(item->name);
	free(item);
}

void
id_table_destroy()
{
	void *key, *data;
	struct hash_table_iter *iter;

	iter = hash_table_iterate_init(identifier);
	
	while (FALSE != hash_table_iterate(iter, &key, &data))
		id_table_destroy_cb((id_table_item_t*)data);

	hash_table_iterate_deinit(&iter);

	hash_table_destroy(&identifier);
}

