#!/usr/bin/python
# -*- coding: utf-8 -*-
import time
import twitter

api = twitter.Api(consumer_key="lalEaBNzKZdUmK2JNc3EVg",
        consumer_secret="your-secret-here",
        access_token_key="your-token-here",
        access_token_secret="your-token-secret")

delay=60

def main():
    lastid = api.GetHomeTimeline(count=1)[0].id
    while True:
        time.sleep(delay)
        lst = api.GetHomeTimeline(count=10, since_id = lastid)
        if len(lst) < 1:
            continue
        if lastid < lst[0].id:
            lastid = lst[0].id
        for item in lst:
            print "initial tweet:%s:%s %s" % (item.created_at, item.user.name, item.text)
main()
