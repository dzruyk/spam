#!/usr/bin/python

import logging
import sys

root = logging.getLogger('botlog')

root.setLevel(logging.DEBUG)

FORMAT="%(asctime)s\t%(message)s"
formatter = logging.Formatter(FORMAT)

fh = logging.FileHandler("sqli.log")
ch = logging.StreamHandler(sys.stderr)

handlers = [fh, ch]

for item in handlers:
    item.setFormatter(formatter)
    item.setLevel(logging.DEBUG)
    root.addHandler(item)

def debug(message, component='root'):
    root.debug("%s\t%s" % (component, message))

def info(message, component='root'):
    root.info("%s\t%s" % (component, message))

def warning(message, component='root'):
    root.warning("%s\t%s" % (component, message))

def error(message, component='root'):
    root.error("%s\t%s" % (component, message))

