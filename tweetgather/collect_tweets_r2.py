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

def get_home_timeline(count = 100, since = None):
    return api.GetHomeTimeline(count, since_id = since)

def collect_hometimeline(dc, count = 100, since = None):
    lst = get_home_timeline(count = count, since = since)
    for item in lst:
        try:
            dc.add_tweet(item)
        finally:
            dc.commit()
    if len(lst) > 0:
        since = lst[0].id

    dc.commit()   

def get_users_timeline(unames, cnt):
    res = []
    lst = api.UsersLookup(screen_name=unames)
    for user in lst:
        #FIXME: add since_id
        res += api.GetUserTimeline(user.id, count = cnt)
    return res

def store_users_timeline(dc, unames, cnt = 100):
    """ store tweets of *unames* userlist
        usage example:
        store_users_timeline(dc, ['justinbieber', 'SashaGrey', 'kevinmitnick'], cnt = 250)
    """
    res = get_users_timeline(unames, cnt)
    for tweet in res:
        ret = dc.add_tweet(tweet)
    dc.commit()

def get_user_friends(uname):
    """
    for friend in get_user_friends('SashaGrey'):
        print friend.name
    """
    lst = api.UsersLookup(screen_name=[uname])
    if len(lst) != 1:
        return []
    return api.GetFriends(user_id=lst[0].id)

def get_user_followers(uname, count=200):
    """Example:
    for follower in get_user_followers('SashaGrey'):
        print friend.name
    """
    lst = api.UsersLookup(screen_name=[uname])
    if len(lst) != 1:
        return []
    return api.GetFollowers(user_id=lst[0].id, count = count)

class UsersIterator():
    def __init__(self, user, n=100):
        self.user = user
        self.n = n
        print "before get followers call"
        self.followers = get_user_followers(user)
        sefl.proceeded = 0
        print "n followers = %d" % self.followers
    def next_friend(self):
        if len(followers) < self.proceed:
            return False
        store_users_timeline(self.followers[self.proceed], n)
        self.proceed += 1
        return True

def main():
    twitter_login()
    dc = tweetdb.DataCollector("testdb")   
    since = None
    print "start of Users iterator"
    u = UsersIterator("habrahabr")

    """
    while True:
        collect_hometimeline(dc)
        time.sleep(delay)
    """

    dc.close()

if __name__ == "__main__":
    main()
