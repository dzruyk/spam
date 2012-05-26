#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <linux/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "list.h"

#define error(str) do {\
		perror(str " ERR:");\
		exit(1);\
		} while(0)

struct _if_descr {
	struct _if_descr *next;
	char *if_name;
	int if_flags;
};

int if_descr_get_all(struct _if_descr **descr);

void if_descr_add_unique(struct _if_descr **root, char *if_name, int if_flags);

void if_descr_free(struct _if_descr *descr);


void print_ifaces(struct _if_descr *ifaces);

boolean_t iface_exist(char *ifname, struct _if_descr *iflist);

void go_sniff(char *ifname);

/*
 * set promisc mode on interface
 * and try to recv data from it
 * mb need to add -l flag for list interfaces
 */

int
main(int argc, char *argv[])
{
	struct _if_descr *iflist;

	char *iface, *pname;
	int opts;
	
	pname = argv[0];
	
	if (if_descr_get_all(&iflist) != 0)
		error("getifaddrs ");
	
	while ((opts = getopt(argc, argv, "lh")) != -1) {
		switch (opts) {
		case 'l':
			print_ifaces(iflist);
			goto err;
			break;
		case 'h':
		default:
			print_usage(pname);
			exit(1);
		}	
	}
	if (argc < 2) {
		print_usage(pname);
		goto err;
	}

	iface = argv[1];
	
	if (iface_exist(iface, iflist) == FALSE) 
		print_warn_and_die("seems to iface not exist\n");
	
	go_sniff(iface);

	if_descr_free(iflist);

	return 0;

err:
	if_descr_free(iflist);

	return 1;
}

int
if_descr_get_all(struct _if_descr **descr)
{
	struct _if_descr *root;

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
if_descr_add_unique(struct _if_descr **root, char *if_name, int if_flags)
{
	struct _if_descr *tmp;

	tmp = malloc_or_die(sizeof(*tmp));
	
	tmp->if_name = ustrdup(if_name);
	tmp->if_flags = if_flags;
	

	//FIXME need to check that current if_name unique
	tmp->next = *root;
	*root = tmp;
}


void
if_descr_free(struct _if_descr *descr)
{
	struct _if_descr *prev;
	
	prev = descr;

	for (; prev != NULL; prev = descr) {
		descr = descr->next;

		//free item
		free(prev->if_name);
		free(prev);
	}
}

//need to write my own func that will be collect unique ifaces
void
print_ifaces(struct _if_descr *ifaces) 
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

boolean_t 
iface_exist(char *ifname, struct _if_descr *iflist)
{
	for (; iflist != NULL; iflist = iflist->next) {
		char *tmp;
		tmp = (char *)iflist->if_name;

		if (strcmp(ifname, tmp) == 0)
			return TRUE;
	}
	
	return FALSE;
}


void
go_sniff(char *ifname)
{
	struct sockaddr_ll from;
	socklen_t len;

	struct ifreq req;
	int i, res;
	int sock;
	char buff[1024];
	
	strcpy(req.ifr_name, ifname);

	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock == -1)
		error("socket");
	
	//get if index
	if (ioctl(sock, SIOCGIFINDEX, &req) != 0)
		error("ioctl get index ");
	
	//bind sock to device
	from.sll_protocol = htons(ETH_P_ALL);
	from.sll_ifindex = req.ifr_ifindex; 
	from.sll_family = AF_PACKET;
	
	if (bind(sock, (struct sockaddr *)&from, sizeof(from)) != 0)
		error("cant bind! ");

	//get flags
	if (ioctl(sock, SIOCGIFFLAGS, &req) != 0)
		error("ioctl ");
	
	req.ifr_flags = req.ifr_flags | IFF_PROMISC;

	//set promisc mode
	if (ioctl(sock, SIOCSIFFLAGS, &req) != 0)
		error("ioctl ");

	len = sizeof(from);
	while ((res = recvfrom(sock, buff, sizeof(buff), 
	                       MSG_TRUNC, (struct sockaddr *) &from, &len)) > 0) {
		
		for (i = 0; i < res; i++)
			printf("%2x ", buff[i]);
		printf("\nnew msg\n");
	}
	
	//unset promisc mode
	req.ifr_flags = req.ifr_flags & (IFF_PROMISC ^ 0xFFffFFff);
	if (ioctl(sock, SIOCSIFFLAGS, &req) != 0)
		error("ioctl ");
	
	close(sock);
}

