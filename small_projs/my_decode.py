#!/usr/bin/env python

import re
import sys
import urllib2
from optparse import OptionParser
from HTMLParser import HTMLParser

baseurl="http://www.lingvo-online.ru/ru/Translate/en-ru/"

def usage():
    print """
    ./program YOU_WORLD in english
    """

def debug(msg):
    print msg

class MyTestParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.recording = 0
        self.data = []
    def handle_data(self, data):
        pass
    def handle_starttag(self, tag, attrs):
        if tag != 'div':
            return
       
        if u'js-article-html g-card' in attrs[0]:
            debug("div finded, attrs %s" %attrs)
    def handle_endtag(self, tag):
        pass

def get_description(url):
    
    try:
        c = urllib2.urlopen(url)
        encoding = c.headers.getparam('charset')
        page = c.read().decode(encoding)
    except KeyboardInterrupt:
        print "[*] Interpreted"
        exit(1)
    except BaseException:
        print "[-] Could not connect to %s" % baseurl
        exit(1)

    p = MyTestParser()
    p.feed(page)
    p.close()

    desc = "Stub now"
    return desc

def create_url(translateme):
    translateme = re.sub("\s+", "%20", translateme)
    url = baseurl + translateme
    return url

def main(argc, argv):
    debug("len of argc = %d" % argc)
    if (argc < 2):
        usage()
        exit(1)
    url = create_url(''.join(argv[1:]))
    debug(url)
    desc = get_description(url)   
    print desc

if __name__=='__main__':
    main(len(sys.argv), sys.argv[:])
