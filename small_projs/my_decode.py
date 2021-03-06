#!/usr/bin/env python
# -*- coding: utf-8
# FIXME: hack to force Python run 
# export PYTHONIOENCODING=UTF-8
import re
import sys
import urllib2
from optparse import OptionParser
from HTMLParser import HTMLParser

#baseurl="http://www.lingvo-online.ru/ru/Translate/en-ru/"
baseurl="http://slovari.yandex.ru/%s/перевод"

def usage():
    print """
    ./program YOU_WORLD in english
    """

types = { u'b-translation__title' : 'title', 
list_useless_types = [u'b-translation__controls']

def debug(msg):
    print msg

class MyTestParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.recording = 0
        self.nesting = 0
        self.data = u[]
    def handle_data(self, data):
        if self.recording == 1:
            self.data += data
        pass
    def determine_start_of_translation(self, tag, attrs):
        """ Now we trying to cutoff part of page """   
        if tag != 'td':
            return
        if len(attrs) == 0:
            return
        for item in attrs:
            if item[0] != u"class":
                return
            if item[1] != u"l-page__center":
                return
        debug("start point, attrs %s" %attrs)
        self.recording = 1
        self.nesting += 1

    def handle_starttag(self, tag, attrs):
        if self.recording == 0:
            self.determine_start_of_translation(tag, attrs)
            return
        self.nesting += 1       
       
    def handle_endtag(self, tag):
        if self.recording == 0:
            return
        self.nesting -= 1
        if self.nesting < 1:
            self.recording = 0

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
    desc = p.data
    return desc

def create_url(translateme):
    translateme = re.sub("\s+", "%20", translateme)
    url = baseurl % translateme
    return url

def main(argc, argv):
    debug("[+] len of argc = %d" % argc)
    if (argc < 2):
        usage()
        exit(1)
    url = create_url(''.join(argv[1:]))
    debug("[+] Created url = %s" % url)
    desc = get_description(url)   
    print type(desc)
    print desc

if __name__=='__main__':
    main(len(sys.argv), sys.argv[:])
