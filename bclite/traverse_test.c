#include <stdio.h>

#include "common.h"
#include "syntax.h"
#include "traverse.h"

int
main(int argc, char *argv[])
{
	syn_tree_t *tree;
	ret_t ret;

	id_table_create();

	do {
		ret = program_start(&tree);
		if (tree == NULL)
			continue;
		if (ret_ok == ret) {
			traverse(tree);
			traverse_print_result();
		}
	} while (syntax_is_eof != 1);

	id_table_destroy();
	
	return 0;
}

