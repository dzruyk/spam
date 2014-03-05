#!/usr/bin/python
# -*- coding: utf-8 -*-
import time
import twitter
import tweetdb

api = None;
delay=60

def twitter_login():
    global api
    api = twitter.Api(consumer_key="lalEaBNzKZdUmK2JNc3EVg",
        consumer_secret="your-secret-here",
        access_token_key="your-token-here",
        access_token_secret="your-token-secret")

def get_timeline(count=100, since = None):
    return api.GetHomeTimeline(count, since_id = since)

def main():
    twitter_login()
    dc = tweetdb.DataCollector("testdb")   
    since = None
    
    while True:
        lst = get_timeline(since = since)
        for item in lst:
            try:
                dc.add_tweet(item)
            finally:
                dc.commit()
        if len(lst) > 0:
            since = lst[0].id

        dc.commit()
        time.sleep(delay)

main()
