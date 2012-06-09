#ifndef __IF_DESCR_H__
#define __IF_DESCR_H__

struct if_descr {
	struct if_descr *next;
	char *if_name;
	int if_flags;
};

/*
 * get description of all interfaces
 */
int if_descr_get_all(struct if_descr **descr);

/*
 * add new unique iface description
 */
void if_descr_add_unique(struct if_descr **root, char *if_name, int if_flags);

/*
 * destroy if_descr structure
 */
void if_descr_free(struct if_descr *descr);


void if_descr_print(struct if_descr *ifaces);

#endif

