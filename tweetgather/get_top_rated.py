#!/usr/bin/python
import twitter

api = twitter.Api(consumer_key="lalEaBNzKZdUmK2JNc3EVg",
        consumer_secret="your-secret-here",
        access_token_key="your-token-here",
        access_token_secret="your-token-secret")

users = api.GetFriends()
for user in users:
    lst = api.GetUserTimeline(user.id, count=50)
    most_retweeted = max(lst, key=lambda item: item.retweet_count)
    print "%s: retweets %d text: %s" % (user.name,
      most_retweeted.retweet_count,
      most_retweeted.text)

