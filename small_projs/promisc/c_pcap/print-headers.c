#include <sys/socket.h>

#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdint.h>

#include "print-headers.h"

enum {
	ETH_LEN = 6,
};

void print_eth_addr(u_int8_t host[ETH_LEN]);

void
print_main(void *data, int len)
{
	print_eth_header(data);
		
	print_raw_hex_data(data, len);

}

/*
 * FIXME:
 * some strange with pppoe
 * 
 */

int
print_eth_header(void *data)
{
	struct ether_header *ether;
	
	ether = data;
	
	printf("Dst ");
	print_eth_addr(ether->ether_dhost);
	printf(" Src ");
	print_eth_addr(ether->ether_shost);
	
	printf("\ntype=%4.4X", ntohs(ether->ether_type));

	printf("\n");
	
	return sizeof(*ether);
}


void
print_raw_hex_data(void *data, int len)
{
	char *buff;
	int i;
	
	buff = (char *)data;

	for (i = 0; i < len; i++) {
		if (i % 8 == 0 && i != 0)
			printf("\n");
		printf("%2.2x", buff[i] & 0xff);
		if (i % 2 == 1 && i % 8 != 0)
			printf(" ");
	}
}

void
print_eth_addr(u_int8_t host[ETH_LEN])
{
	int i;

	for (i = 0; i < 6; i++) {
		printf("%2.2X", host[i]);
		if (i != ETH_LEN - 1)
			printf(":");
	}

}

