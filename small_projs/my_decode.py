#!/usr/bin/env python

from optparse import OptionParser
import re
import sys
import urllib2


baseurl="http://www.lingvo-online.ru/ru/Translate/en-ru/"

def usage():
    print """
    ./program YOU_WORLD in english
    """

def debug(msg):
    print msg

def get_description(url):
    
    try:
        page = urllib2.urlopen(url).read()
    except KeyboardInterrupt:
    except BaseException:

def create_url(translateme):
    translateme = re.sub("\s+", "%20", translateme)
    url = baseurl + translateme
    return url

def main(argc, argv):
    if (argc < 2):
        usage()
        exit(1)
    url = create_url(''.join(argv[1:]))
    debug(url)
    desc = get_description(url)   
    print desc

if __name__=='__main__':
    main(len(sys.argv), sys.argv[:])
