#!/usr/bin/env python

from getopt import gnu_getopt as getopt
import random
import sys

def print_warn_and_die(s):
	print s;
	sys.exit(1);

def print_usage_and_die(pname):
	print "USAGE:\n %s [-h] src_file [dest_file]" % pname;
	usage =  "%s try to generate pairs: string-substring\n" % pname;
	usage = usage + " for every input string from file and print it to\n";
	usage = usage + " stdout or dest file";

	print usage;

	sys.exit(1);

def main():

	(opts, rest) = getopt(sys.argv[1:], "h");

	for opt in opts:
		if opt == "-h":
			print_usage_and_die(sys.argv[0]);
		elif opt != "":
			print_usage_and_die(sys.argv[0]);
	
	if len(rest) > 2 or len(rest) == 0:
		print_usage_and_die(sys.argv[0]);

	try:
		fp = open(rest[0], "r");
		if len(rest) == 2:
			dest = open(rest[1], "w");
		else:
			dest = sys.stdout;
	except:
		print_warn_and_die("file open err\n");
	
	for s in fp:
		s = s.replace('\n', "");
		slen = len(s);

		if slen == 0:
			continue;

		start = random.randint(0, slen - 1);	
		stop = random.randint(start, slen);

		subs = s[start:stop];

		dest.write(s + '\n');
		dest.write(subs + '\n');

if __name__ == "__main__":
	main();
