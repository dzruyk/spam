#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <linux/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <signal.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "if_descr.h"
#include "print-headers.h"

#define PACKET_SZ 66000

#define error(str) do {\
		perror(str " ERR:");\
		exit(1);\
		} while(0)


struct sock_state {
	struct sock_state *next;
	int sock;
	struct ifreq *req;
} *states;

void hold_sock_state(int sock, const struct ifreq *req);

void restore_sock_state_all();

void abrt_handler(int signal);

boolean_t iface_exist(char *ifname, struct if_descr *iflist);

void go_sniff(char *ifname);


/*
 * Set promisc mode on interface
 * and try to recv data from it
 */

int
main(int argc, char *argv[])
{
	struct if_descr *iflist;

	char *iface, *pname;
	int opts;
	
	pname = argv[0];
	
	//get iface description
	if (if_descr_get_all(&iflist) != 0)
		error("getifaddrs ");

	signal(SIGTERM, abrt_handler);
	signal(SIGABRT, abrt_handler);
	signal(SIGINT, abrt_handler);

	while ((opts = getopt(argc, argv, "lh")) != -1) {
		switch (opts) {
		case 'l':
			if_descr_print(iflist);
			if_descr_free(iflist);
			exit(0);
			break;
		case 'h':
		default:
			print_usage(pname);
			exit(1);
		}	
	}

	if (argc < 2) {
		print_usage(pname);
		exit(1);	
	}

	iface = argv[1];
	
	if (iface_exist(iface, iflist) == FALSE) 
		print_warn_and_die("seems to iface not exist\n");
	
	go_sniff(iface);

	if_descr_free(iflist);

	return 0;
}

/*
 * check that ifname exists in the iflist
 * RETURNS:
 * 	TRUE if finds
 * 	FALSE if can't find
 */

boolean_t 
iface_exist(char *ifname, struct if_descr *iflist)
{
	for (; iflist != NULL; iflist = iflist->next) {
		char *tmp;
		tmp = (char *)iflist->if_name;

		if (strcmp(ifname, tmp) == 0)
			return TRUE;
	}
	
	return FALSE;
}


/*
 * FIXME: splitme
 *
 * this is main function
 * it set promisc mode on interface and captures packets
 * after end of capture it must unset promisc mode(not yet implemented)
 */

void
go_sniff(char *ifname)
{
	struct sockaddr_ll from;
	struct msghdr hdr;
	struct iovec iov;
	struct ifreq req;

	int res;
	int sock;
	char buff[PACKET_SZ];


	//set name of iface
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
	
	hold_sock_state(sock, &req);

	//set promisc mode
	req.ifr_flags = req.ifr_flags | IFF_PROMISC;
	
	if (ioctl(sock, SIOCSIFFLAGS, &req) != 0)
		error("ioctl ");


	memset(&hdr, 0, sizeof(hdr));
	hdr.msg_iov = &iov;
	hdr.msg_iovlen = 1;
	iov.iov_base = buff;
	iov.iov_len = sizeof(buff);

	while (TRUE) {
		
		res = recvmsg(sock, &hdr, 0);
		if (res <= 0) 
			break;

		printf("\nnew msg\n");
		
		print_main(buff, res);

		printf("\n");
	}
	
	//unset promisc mode
	restore_sock_state_all();

	close(sock);
}


/*
 * we need old sock state when we finish sniffing 
 */
void
hold_sock_state(int sock, const struct ifreq *req)
{
	struct sock_state *tmp;

	tmp = malloc_or_die(sizeof(*tmp));

	tmp->sock = sock;
	tmp->req = malloc_or_die(sizeof(*(tmp->req)));
	memcpy(tmp->req, req, sizeof(*req));

	tmp->next = states;
	states = tmp;
}

void
restore_sock_state_all()
{
	struct sock_state *tmp, *prev;

	for (prev = tmp = states; tmp != NULL; prev = tmp) {
		//unset saved flags^___^ on socket
		printf("sock = %d\n",prev->sock);
		if (ioctl(prev->sock, SIOCSIFFLAGS, prev->req) != 0)
			error("ioctl\n");

		tmp = tmp->next;
		free(prev->req);
		free(prev);
	}
}

/*
 * simple signal handler
 */
void 
abrt_handler(int signal)
{
	restore_sock_state_all();
	
	printf("Catch signal, dying...\n");
	
	exit(1);
}

