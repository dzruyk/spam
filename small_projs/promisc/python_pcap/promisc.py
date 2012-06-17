#!/usr/bin/env python

import getopt
import pcapy
import signal
import sys

from print_headers import *

def print_usage_and_die():
	print "USAGE:\n%s [-l] interface" % sys.argv[0];
	sys.exit(1);


def abrt_handler(signum, stack):
	print "catch signal, dying...\n";
	sys.exit(1);


def main():
	try: 
		(opts, argu) = getopt.getopt(sys.argv[1:], "l");
	except getopt.GetoptError:
		print "option error";
		print_usage_and_die();
	
	signal.signal(signal.SIGTERM, abrt_handler);
	signal.signal(signal.SIGABRT, abrt_handler);
	signal.signal(signal.SIGINT, abrt_handler);

	# try to search -l opt
	for tmp in opts:
		if tmp[0] == '-l':
			try:
				devs = pcapy.findalldevs();
			except:
				print "No interfaces available\n May be you not a root?";
				sys.exit(0);
			i = 1;
			for dev in devs:
				print "%d) %s" % (i, dev);
				i = i + 1;
			sys.exit(0);
	
	if len(argu) != 1:
		print "wrong number of params";
		print_usage_and_die();
	
	# try to sniff...
	# when timeout = 0 programm cant handle signals(
	try:
		iface = pcapy.open_live(argu[0], 65535, True, 1);
	except pcapy.PcapError as err:
		print err;
		sys.exit(1);
	
	# catch and print packets
	while True:
		try:
			(hdr, payload) = iface.next();
		except:
			continue;
		sys.stdout.write("new msg\n");

		print_eth_header(payload);
		print_raw_hex_data(payload);


if __name__ == "__main__":
	main();


