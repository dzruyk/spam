#!/usr/bin/python

import commands as CMD
import threading
import Queue
from optparse import OptionParser
import sys
import logging
from logging import debug

#logging.basicConfig()
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
maxthreads = 10
verboselvl = 0

def usage():
    print """Simple Paralell Slave Server Command executor
Usage:

%s (-l host_list | -o one_host)  (-c command | script)

Where:
    host_list is \\n delimited list of hosts alias. For Example:
    
    minfin
    ricoko
    etc...

""" % sys.argv[0]

def parse_options():
    global commands, hosts, maxthreads, verboselvl
    parser = OptionParser()
    parser.description = "Simple Slave Server Command executor"
    parser.add_option('-l', '--host_list', dest="host_list", help="delimited list of hosts alias.")
    parser.add_option('-o', '--host', dest="host", help="only one host")
    parser.add_option('-c', '--command', dest="command", help="command to execute")
    parser.add_option('-n', '--nthreads', dest="nthreads", help="number of parralell threads")
    parser.add_option('-v', '--verbose', dest="verbose", action="store_true", help="verbose level")
    parser.print_help = usage
    (options, args) = parser.parse_args()

    if len(args) > 0:
        try: commands = open(args[0]).read().rstrip('\n')
        except Exception as e: print e; exit(1) 
    if options.command is not None:
        commands = options.command
    if options.host_list is not None:
        hosts = open(options.host_list).read().rstrip('\n')
    if options.host is not None:
        hosts = options.host
    if options.nthreads is not None:
        maxthreads = int(options.nthreads)
    maxthreads = min(maxthreads, len(hosts.splitlines()))
    if options.verbose:
        logging.basicConfig(level = logging.DEBUG)
    #sanity check
    if commands is None or len(commands) == 0:
        print 'can\'t find any commands';
        exit(1)
    if hosts is None:
        usage()
        exit()
    print hosts.splitlines()
    print "%s threads" % maxthreads

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
    print "[+] host_worker started on %s" % client
    opts = config.lookup(client)

    if not is_port_opened(opts):
        return client +":\n" + "Connection error\n\n"
    ssh_client = SSHClient()
    ssh_client.load_system_host_keys()
    ssh_client.set_missing_host_key_policy(AutoAddPolicy())

    print "before connection"
    ssh_client.connect(opts['hostname'],
      username=opts['user'],
      key_filename=opts['identityfile'])
    print "after connection"
    stdin, stdout, stderr = ssh_client.exec_command(commands);
    output = stdout.read().rstrip('\n')
    ssh_client.close()
    print "[+] host_worker finished on %s" % client
    return client + ":\n" + output + "\n\n"

def thread_worker(hosts_queue, result_queue, commands):
    while not is_end or not hosts_queue.empty():
        print "[+] next iter"
        try: host = hosts_queue.get(True, 0.5) #wait before next iter
        except: continue;
        out = host_worker(host, commands)
        result_queue.put(out);

def main(argc, argv):
    global config, is_end, maxthreads
    if (argc < 2):
        usage()
        exit(1)

    parse_options()

    debug("Arguments: maxthreads %d" % (maxthreads))

    config = SSHConfig()
    config.parse(open('/root/.ssh/config'))
    
    hosts_queue = Queue.Queue()
    result_queue = Queue.Queue()
    threads = [
      threading.Thread(target = thread_worker,
        args = (hosts_queue, result_queue, commands))
      for i in range(maxthreads)]

    print "[+] initialisation complete"
    for i in range(maxthreads):
        threads[i].daemon = True
        threads[i].start()
    print "[+] Threads created"
    #print "[+] cmd is %s" % commands
    for host in hosts.splitlines():
        if len(host) == 0: continue
        hosts_queue.put(host)
        print host
    print "[+] Host queue filled"
    #No data for workes avaiable, set finalize flag
    is_end = True
    for t in threads:
        print "wait another thread"
        t.join()
    while not result_queue.empty():
        print result_queue.get()

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
