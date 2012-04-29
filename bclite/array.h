#ifndef ARRAY_H_
#define ARRAY_H_

#include "common.h"

typedef struct {
	int n;
	int item_sz;
	void *ptr;
} arr_t;

arr_t *arr_new(int n, int item_sz);

ret_t arr_set_item(arr_t *arr, int ind, int value);

void arr_free(arr_t *arr);

void arr_print(arr_t *arr);

#endif

