#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "id_table.h"
#include "lex.h"
#include "macros.h"

#define INITIAL_SZ 32

struct id_tables {
	struct id_tables *prev;
	struct hash_table *scope;
};

struct id_tables *scopes;

struct hash_table *global;
struct hash_table *current;


static struct hash_table *id_table_create();

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

static void
id_table_destroy_cb(id_table_item_t *item)
{
	free(item->name);
	free(item);
}

void
id_tables_init()
{
	if (scopes != NULL)
		print_warn_and_die("double id table initialisation\n");
	scopes = malloc_or_die(sizeof(*scopes));
	memset(scopes, 0, sizeof(*scopes));

	global = current = id_table_create();
	
	scopes->scope = global;
}

static struct hash_table *
id_table_create()
{
	struct hash_table *table;

	table = hash_table_new(INITIAL_SZ, (hash_callback_t)id_hash_cb,
	    (hash_compare_t)id_compare);
	
	if (table == NULL)
		print_warn_and_die("error at table table creation\n");
	
	return table;
}

ret_t
id_table_insert(id_table_item_t *item)
{
	int res;
	
	res = hash_table_insert_unique(current, item->name, item);
	if (res == ret_out_of_memory)
		print_warn_and_die("error at id table insertion\n");
	else if (res == ret_entry_exists)
		print_warn_and_die("internal error, entry exists\n");

	return ret_ok;
}

id_table_item_t *
id_table_lookup(char *name)
{
	id_table_item_t *res;
	
	if (hash_table_lookup(current, name, (void**)&res) != ret_ok)
		return NULL;
	else
		return res;
}


void
id_table_free(struct hash_table *table)
{
	void *key, *data;
	struct hash_table_iter *iter;

	iter = hash_table_iterate_init(table);
	
	while (hash_table_iterate(iter, &key, &data) != FALSE)
		id_table_destroy_cb((id_table_item_t*)data);

	hash_table_iterate_deinit(&iter);

	hash_table_destroy(&table);
}

void
id_tables_destroy()
{
	struct id_tables *tmp;

	for (; scopes != NULL; scopes = tmp) {
		tmp = scopes->prev;
		id_table_free(scopes->scope);
		free(scopes);
	}
}

