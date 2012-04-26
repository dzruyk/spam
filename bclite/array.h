#ifndef ARRAY_H_
#define ARRAY_H_

typedef struct {
	//может стоит переть тип из id_table
	int type;
	int nitem;
	int item_sz;
	void *ptr;
} arr_t;

#endif

