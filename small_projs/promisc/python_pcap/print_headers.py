#!/usr/bin/env python

import sys
from socket import ntohs;

#global_vars
ETHER_LEN = 6

stdwrite = sys.stdout.write;

def print_ether_addr(addr):
	for i in range(ETHER_LEN):
		stdwrite("%2.2X" % addr[i]);
		if i != ETHER_LEN - 1:
			stdwrite(":");

class ether_header:
	def __init__(self, data):
		self.ether_dhost = [ord(addr) for addr in data[0:6]];
		self.ether_shost = [ord(addr) for addr in data[6:12]];
		self.ether_type = ord(data[12]);
	
	def print_dhost(self):
		print_ether_addr(self.ether_dhost);

	def print_shost(self):
		print_ether_addr(self.ether_shost);
	
	def print_type(self):
		stdwrite("%4.4X" % ntohs(self.ether_type));


def print_eth_header(payload):
	hdr = ether_header(payload);
	stdwrite("Dst ");
	hdr.print_dhost();
	stdwrite(" Src ");
	hdr.print_shost();
	stdwrite("\ntype=");
	hdr.print_type();
	stdwrite("\n");


def print_raw_hex_data(payload):
	for i in range(len(payload)):
		if i % 8 == 0 and i != 0:
			stdwrite("\n");
		stdwrite("%2.2X" % ord(payload[i]));
		if i % 2 == 1 and i % 8 != 0:
			stdwrite(" ");
	stdwrite("\n\n");

