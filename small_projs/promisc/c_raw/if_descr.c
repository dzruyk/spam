#include <sys/types.h>

#include <ifaddrs.h>
#include <linux/if.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "if_descr.h"

int
if_descr_get_all(struct if_descr **descr)
{
	struct if_descr *root;

	struct ifaddrs *iflst, *ifitem;

	if (getifaddrs(&iflst) != 0)
		return -1;
	
	root = NULL;

	for (ifitem = iflst; ifitem != NULL; ifitem = ifitem->ifa_next)
		if_descr_add_unique(&root, ifitem->ifa_name, ifitem->ifa_flags);
	
	*descr = root;

	freeifaddrs(iflst);

	return 0;
}

void
if_descr_add_unique(struct if_descr **root, char *if_name, int if_flags)
{
	struct if_descr *nitem, *tmp;

	//check that entry not exist in *root list
	for (tmp = *root; tmp != NULL; tmp = tmp->next)
		if (strcmp(if_name, tmp->if_name) == 0)
			return;

	nitem = malloc_or_die(sizeof(*nitem));
	
	nitem->if_name = ustrdup(if_name);
	
	nitem->if_flags = if_flags;
	nitem->next = *root;
	*root = nitem;
}

void
if_descr_free(struct if_descr *descr)
{
	struct if_descr *prev;
	
	prev = descr;

	for (; prev != NULL; prev = descr) {
		descr = descr->next;

		//free item
		free(prev->if_name);
		free(prev);
	}
}


void
if_descr_print(struct if_descr *ifaces) 
{
	int i;
	
	for (i = 0; ifaces != NULL; i++, ifaces = ifaces->next) {
		char *name;

		name = (char *)ifaces->if_name;
		if ((ifaces->if_flags & IFF_UP) == 0)
			continue;

		printf("%d) %s", i + 1, name);
		
		printf("\n");
	}
}

