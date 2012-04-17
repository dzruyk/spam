#ifndef __LIST_H__
#define __LIST_H__

struct list {
	struct list *next;
	void *data;
};

typedef struct list list_t;

#endif

