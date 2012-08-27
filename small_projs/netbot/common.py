#!/usr/bin/env python

DEBUG = 1

def_port = 5554
def_ip = "0.0.0.0"

SERV_IP = "127.0.0.1"
SERV_PORT = 5554

PACK_SZ=512
HELLO_MSG = "HELLO\n"
HELLO_ACK = "HELLO/ACK"

def D(stmt):
	"""
	helper function, try to resolve debug value,
	if success, print debug message
	"""
	try:
		DEBUG
	except:
		return;
	exec(stmt);

