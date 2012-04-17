#include <stdio.h>

#include "common.h"
#include "id_table.h"
#include "keyword.h"
#include "syntax.h"
#include "syn_tree.h"
#include "traverse.h"

int
main(int argc, char *argv[])
{
	syn_tree_t *tree;
	ret_t ret;

	id_tables_init();
	keyword_table_init();

	do {
		ret = program_start(&tree);
		if (tree == NULL || ret != ret_ok)
			continue;
		if (traverse_prog(tree) == ret_ok)
			traverse_print_result();

	} while (syntax_is_eof != 1);

	id_tables_destroy();
	
	keyword_table_destroy();

	return 0;
}

