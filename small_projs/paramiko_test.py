#!/usr/bin/python

import commands as CMD
import threading
import Queue
from optparse import OptionParser
import sys

try:
    from paramiko import SSHClient, SSHConfig, AutoAddPolicy
except:
    print "Can't get paramiko. It's rly installed?"
    exit(1)

#global SSH Config object
config = None

commands = None
hosts = None
is_end = False

def usage():
    print """

    Simple Slave Server Command executor
    Usage:
    
    %s -l host_list  (-c command | script)

    Where:
    """ % sys.argv[0]

def parse_options():
    global commands, hosts
    parser = OptionParser()
    parser.add_option('-l', '--host_list', dest="host_list")
    parser.add_option('-c', '--command', dest="command")
    parser.print_help = usage
    (options, args) = parser.parse_args()
    if len(args) > 0:
        try: commands = open(args[0]).read().rstrip('\n')
        except Exception as e: print e; exit(1) 
    if options.command is not None:
        commands = options.command
    if options.host_list is not None:
        hosts = open(options.host_list).read().rstrip('\n')
    #sanity check
    if commands is None or len(commands) == 0:
        print 'can\'t find any commands';
        exit(1)
    if hosts is None:
        usage()
        exit()

def is_port_opened(options):
    #dirty nmap hack
    if options.has_key('port') is not None:
        options['port'] = 22

    exec_string = "nmap -sS -p %s %s | grep open" % (options['port'], options['hostname'])
    out = CMD.getoutput(exec_string)
    if len(out) == 0:
        return False
    return True

def host_worker(client, commands):
    opts = config.lookup(client)

    if not is_port_opened(opts):
        return client +":\n" + "Connection error\n\n"
    ssh_client = SSHClient()
    ssh_client.load_system_host_keys()
    ssh_client.set_missing_host_key_policy(AutoAddPolicy())

    ssh_client.connect(opts['hostname'],
      username=opts['user'],
      key_filename=opts['identityfile'])

    stdin, stdout, stderr = ssh_client.exec_command(commands);
    output = stdout.read().rstrip('\n')
    ssh_client.close()
    return client + ":\n" + output + "\n\n"

def thread_worker(hosts_queue, result_queue, commands):
    while not is_end or not hosts_queue.empty():
        try: host = hosts_queue.get(True, 1) #wait 2 seconds before next iter
        except: continue;
        out = host_worker(host, commands)
        result_queue.put(out);

def main(argc, argv):
    global config, is_end
    if (argc < 2):
        usage()
        exit(1)
    parse_options()
    
    maxthreads = 5
    config = SSHConfig()
    config.parse(open('/root/.ssh/config'))
    
    hosts_queue = Queue.Queue()
    result_queue = Queue.Queue()
    threads = [
      threading.Thread(target = thread_worker,
        args = (hosts_queue, result_queue, commands))
      for i in range(maxthreads)]

    for i in range(maxthreads):
        threads[i].daemon = True
        threads[i].start()
    
    #print "[+] cmd is %s" % commands
    for host in hosts.splitlines():
        if len(host) == 0: continue
        hosts_queue.put(host)
        print host
    
    #No data for workes avaiable, set finalize flag
    is_end = True
    for t in threads:
        print "wait another thread"
        t.join()
    while not result_queue.empty():
        print result_queue.get()

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
