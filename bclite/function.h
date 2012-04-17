#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include "list.h"

//FIXME: return value?
typedef struct {
	int narg;
	list_t *args;
	//КОСТЫЛЬ!!!!!!!!!!
	void *body;
	void *scope;
} function_t;

function_t *function_new();

#endif

