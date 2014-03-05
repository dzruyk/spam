#!/usr/bin/python

import pysqlite2.dbapi2 as dbapi

condelay = 10

#WARN: rewrite me
class DataCollector:

    def __init__(self, dbfname):
        self.connect("testdb")
    
    def connect(self, fname):
        self.con = dbapi.connect("testdb", timeout=condelay)

    def add_hashtag(self, hashtag):
        r = self.con.execute("select * from hashtags where txt = ?",
                (hashtag.text,)).fetchall()
        if (len(r) > 0):
            return False

        self.con.execute("insert into hashtags (txt) values (?)",
                (hashtag.text,))
        return True

    def get_hashtag(self, hashtag):
        return self.con.execute("select * from hashtags where txt = ?",
                (hashtag.text,)).fetchall()

    def add_tweet_hashtags(self, tid, hashtags, usr_id):
        for item in hashtags:
            self.add_hashtag(item)
            hid = self.get_hashtag(item)[0][0]
            r = self.con.execute("insert into tweet_hashtags " +
                    "(tid, hid, usr_id) values (?, ?, ?)", (tid, hid, usr_id,))
        return True

    def add_user(self, user):
        r = self.con.execute("select * from users where name = ?",
                (user.name,)).fetchall()
        if (len(r) > 0):
            return False

        self.con.execute("insert into users (usr_id, name, friends_cnt) values (?, ?, ?)",
                (user.id, user.name, user.friends_count,))
        return True

    def update_user(self, user):
        self.con.execute("update users set friends_cnt =? where usr_id = ?", 
                (user.friends_count, user.id,))
        return True

    def add_tweet(self, tweet):
        r = self.con.execute("select * from tweets where tid = ?", (tweet.id,)).fetchall()
        if (len(r) > 0):
            return False

        self.add_user(tweet.user)

        self.con.execute("insert into tweets (tid, usr_id, text) values (?, ?, ?)",
                (tweet.id, tweet.user.id, tweet.text,))
        if (len(tweet.hashtags) < 1):
            return True

        self.add_tweet_hashtags(tweet.id, tweet.hashtags, tweet.user.id)
        return True

    def get_last_tweet(self):
        r = self.con.execute("SELECT * FROM users "
                "WHERE usr_id IN ("
                    "SELECT MAX(usr_id) FROM users"
                ")")
        return r.fetchall()

    def commit(self):
        self.con.commit()

    def close(self):
        self.con.close()

    def flush_it(self):
        """ WARN: use with care"""
        cursor = self.con.execute("select tbl_name " +
                "from main.sqlite_master " +
                "where type = 'table'")
        for entry in cursor:
            self.con.execute("delete from ?", (entry[0],))

def test_it():
    dc = DataCollector("./testdb")
    dc.flush_it()

if __name__ == "__main__":
    test_it()
