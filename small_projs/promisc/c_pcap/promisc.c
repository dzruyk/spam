#include <pcap/pcap.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "print-headers.h"

void print_devs(pcap_if_t *devs);

void go_sniff(char *ifname);

int
main(int argc, char *argv[])
{
	char errbuff[PCAP_ERRBUF_SIZE];
	char *iface, *pname;
	int err, opts;

	pcap_if_t *devs;

	pname = argv[0];
 
	while ((opts = getopt(argc, argv, "lh")) != -1) {
		switch (opts) {
		case 'l':
			err = pcap_findalldevs(&devs, errbuff);
			if (err != 0)
				print_warn_and_die("pcap find all devs err: %s\n", errbuff);
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

	if (argc < 2) {
		print_usage(pname);
		exit(1);	
	}
	
	iface = argv[1];
	
	// Try to sniff
	go_sniff(iface);

	return 0;
}


void
go_sniff(char *ifname)
{
	char errbuff[PCAP_ERRBUF_SIZE];
	const char *data;

	struct pcap_pkthdr h;

	pcap_t *pfd;


	pfd = pcap_open_live(ifname, 65535, TRUE, 10, errbuff);

	if (pfd == NULL)
		print_warn_and_die("pcap_open_live err\n");
	printf("gosniff\n");
	while (TRUE) {
		if ((data = pcap_next(pfd, &h)) == NULL)
			continue;
		printf("\nnew msg\n\n");
		print_main((void *)data, h.len);

	}
	pcap_close(pfd);
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

