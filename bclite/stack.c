#include <stdlib.h>

#include "stack.h"
#include "helper_funcs.h"

//FIXME: may be need to implement stack with dynamic array
//	 but I'm so tired...)

struct lst {
	struct lst *next;
	void *data;
};

struct lst *stack = NULL;

void 
stack_push(void *data)
{
	struct lst *nitem;

	if (data == NULL)
		print_warn_and_die("INTERNAL_ERROR: try to push NULL in stack\n");
	
	nitem = malloc_or_die(sizeof(*nitem));
	
	nitem->data = data;
	nitem->next = stack;
	stack = nitem;
}

void *
stack_pop()
{
	struct lst *nitem;
	void *data;

	if (stack == NULL)
		return NULL;
	
	nitem = stack;
	stack = stack->next;

	data = nitem->data;
	
	free(nitem);
	return data;
}

void 
stack_flush(stack_item_free_t ifree)
{
	struct lst *next;

	for (; stack != NULL; stack = next) {
		next = stack->next;
		ifree(stack->data);
		free(stack);
	}
}

