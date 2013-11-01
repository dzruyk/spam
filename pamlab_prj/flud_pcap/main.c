#include <getopt.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <unistd.h>

#include "shape_packet.h"

#define error(status, fmt, arg...) \
do { \
	fprintf(stderr, "error: %s:%d %s: "fmt, \
			__FILE__, \
			__LINE__, \
			__FUNCTION__, \
			##arg); \
	fflush(stderr); \
	exit(status);	\
} while (0)

#define FALSE 0
#define TRUE !FALSE

void print_usage(char *pname);
void print_devs(pcap_if_t *devs);

void
send_loop(char *device)
{
	my_packet_t packet;
	char errbuf[PCAP_ERRBUF_SIZE];
	/* see pcap(3) why */
	int snaplen = 65535;
	int timeout = 100;
	pcap_t *pcap;

	pcap = pcap_open_live(device, snaplen, FALSE, timeout, errbuf);
	if (pcap == NULL)
		error(1, "can't open %s device\n", device);

	while (1) {
		int max_len = sizeof(packet);

		shape_packet(&packet, max_len);
		set_payload(&packet, max_len);

		pcap_inject(pcap, &packet, max_len);
		//usleep(1000 * 100);
	}

	pcap_close(pcap);
}


int
main(int argc, char *argv[])
{
	char errbuf[PCAP_ERRBUF_SIZE];
	char *iface, *pname;
	int err, opts;

	pcap_if_t *devs;

	pname = argv[0];

	while ((opts = getopt(argc, argv, "lh")) != -1) {
		switch (opts) {
		case 'l':
			err = pcap_findalldevs(&devs, errbuf);
			if (err != 0)
				error(1, "pcap find all devs err: %s\n", errbuf);
			else
				print_devs(devs);
			exit(0);
			break;
		case 'h':
		default:
			print_usage(pname);
			exit(1);
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 1) {
		print_usage(pname);
		exit(1);
	}

	iface = argv[0];

	send_loop(iface);

	return 0;
}

void
print_usage(char *pname)
{
	printf("USAGE\n"
	       "%s -l ifname\n"
	       "\n"
	       "send flood multiple packages with different source mac addresses via ifname\n", pname);

}

void
print_devs(pcap_if_t *devs)
{
	int i;

	i = 0;

	if (devs == NULL)
		printf("No available ifaces(Are you root?\n");

	while (devs != NULL) {
		i++;
		printf("%d) %s\n", i, devs->name);

		devs = devs->next;
	}
}


