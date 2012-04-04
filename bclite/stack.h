#ifndef _STACK_H_
#define _STACK_H_

typedef void (*stack_item_free_t)(void *item);


void stack_push(void *data);

void *stack_pop();

void stack_flush(stack_item_free_t ifree);

#endif

