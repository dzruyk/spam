#include <stdlib.h>

#include "helper_funcs.h"

//FIXME: may be need to implement stack with dynamic array
//	 but I'm so tired...)
//FIXME 2: need to replace (int val) element with (void *val)
//

struct lst {
	struct lst *next;
	int val;
};

struct lst *stack = NULL;

void 
stack_push(int val)
{
	struct lst *nitem;

	//if (NULL == val)
	//	print_warn_and_die("INTERNAL_ERROR: try to push NULL in stack\n");
	
	nitem = malloc_or_die(sizeof(*nitem));
	
	nitem->val = val;
	nitem->next = stack;
	stack = nitem;
}

int
stack_pop()
{
	struct lst *nitem;
	int val;

	if (NULL == stack)
		print_warn_and_die("INTERNAL_ERROR: stack is empty\n");
	
	nitem = stack;
	stack = stack->next;

	val = nitem->val;
	
	free(nitem);
	return val;
}

void 
stack_flush()
{
	struct lst *next;

	for (; NULL != stack; stack = next) {
		next = stack->next;
		free(stack);
	}
}



