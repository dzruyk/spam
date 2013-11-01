#include <net/ethernet.h>
#include <netinet/ip.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "shape_packet.h"

static void
gen_rand_mac(u_int8_t addr[ETH_ALEN])
{
	int i;

	//srand((NULL));

	for (i = 0; i < ETH_ALEN; i++)
		addr[i] = rand() % 0xff;
}

void
shape_packet(my_packet_t *packet, int max_len)
{
	int i;

	memset(packet, 0, max_len);

	packet->ether.ether_type = htons(0x800);
	gen_rand_mac(packet->ether.ether_shost);
	/* brodcast */
	for (i = 0; i < ETH_ALEN; i++)
		packet->ether.ether_dhost[i] = 0xff;
	/* FIXME: fill ip structure */
	packet->ip.ihl = 5;
	packet->ip.version = 4;
	packet->ip.tot_len = htons(20);
	packet->ip.ttl = 0x20;
	packet->ip.protocol = 0x06;
}

void
set_payload(my_packet_t *packet, int max_len)
{
	/*FIXME: Writeme */
}
