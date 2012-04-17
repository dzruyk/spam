#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef struct {
	//может стоит переть тип из id_table
	int type;
	int nitem;
	int item_sz;
	void *ptr;
} arr_t;

#endif

