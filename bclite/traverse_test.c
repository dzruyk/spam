#include <stdio.h>

#include "id_table.h"
#include "syntax.h"
#include "traverse.h"

int
main(int argc, char *argv[])
{
	syn_tree_t *tree;

	id_table_create();

	do {
		tree = program_start();
		traverse(tree);
		print_result();
	} while (tree != NULL);

	id_table_destroy();
	
	return 0;
}


