#!/usr/bin/env python

import random
import sys

def print_warn_and_die(s):
	print s;
	sys.exit(1);

def main():
	try:
		fp = open("text.txt", "r");
		dest = open("res.txt", "w");
	except:
		print_warn_and_die("file open err\n");
	
	for s in fp:
		s = s.replace('\n', "");
		slen = len(s);
		start = random.randint(0, slen - 1);	
		stop = random.randint(start, slen);

		subs = s[start:stop];

		dest.write(s + '\n');
		dest.write(subs + '\n');

if __name__ == "__main__":
	main();
