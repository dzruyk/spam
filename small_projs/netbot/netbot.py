#!/usr/bin/env python

import errno
import socket
import time

from common import *

CONNECT_SLEEP_TIME = 1
MAX_CONNECTION_TRY = 2


class netbot:
	""" 
	netbot - someone who listen on port messages
	of creator and try to handle this messages for his
	suspicious goals
	"""
	sock = None
	def __init__(self, what_ip=def_ip, what_port = def_port):
		self.port = what_port
		self.ip = what_ip
		D("print '[+]netbot initialisation...'");
	
	def run(self):
		i = 0
		while self.connect() != True:
			time.sleep(CONNECT_SLEEP_TIME)
			i = i + 1
			if i > MAX_CONNECTION_TRY:
				return

		self.sock.setblocking(1);

		try:
			self.sock.sendall(HELLO_MSG);
			ack = self.sock.recv(PACK_SZ, socket.MSG_DONTWAIT)
		except Exception as err:
			D('print "[-]recv HELLO/ACK fail"');
			print err
			return False;
			
		if ack != HELLO_ACK:
			print ack
			D('print "[-]netbot ack from server incorrect"');
			return False;

		D('print "[+] Hello/ACK recieved"');

		while True:
			try:
				ack = self.sock.recv(PACK_SZ, socket.MSG_DONTWAIT)
				print ack
				time.sleep(5);
				break;
			except Exception as err:
				
				D('print "recv fail"');
				print err;
				return False;

	
	def send_msg(self, msg):
		pass;
	
	def connect(self):
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

		try:
				co = (self.ip, self.port)
				self.sock.connect(co)
		except Exception as err:
				D("print '[-] socket connect fail'");
				print err
				self.disconnect()
				return False;
		
		D("print '[+] socket.connect success'");
		
		return True;
	
	def disconnect(self):
		if self.sock != None:
			self.sock.close()
		self.sock = None
		pass;
	
	def __del__(self):
		self.disconnect()
		pass;



def test_bot():
	"""
	simple testing routine
	"""
	mybot = netbot();
	mybot.run();
	
	
	del(mybot)

if __name__ == "__main__":
	test_bot()