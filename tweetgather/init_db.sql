create table hashtags (
	hid INTEGER,
	txt TEXT not NULL,
	primary key (hid) 
);

create table tweet_hashtags (
	tid INTEGER NOT NULL,
	hid INTEGER NOT NULL,
	usr_id INTEGER NOT NULL,
	foreign key (usr_id)
	  references users (usr_id),
	foreign key (hid)
	  references hashtags (hid),
	foreign key (tid)
	  references tweets (tid),
	primary key (tid, hid, usr_id)
);

create table users (
	usr_id INTEGER,
	name TEXT not NULL,			--twitter user account name

	friends_cnt INTEGER default 0,
	followers_cnt INTEGER default 0,
	favorites_cnt INTEGER default 0,
	created_at TEXT default '',
	profile_image_url TEXT default '',

	usr TEXT default '',			--deserialized python object (for backup)
	primary key (usr_id)
);

create table tweets (
	tid INTEGER,				--twitter id
	usr_id INTEGER,
	text TEXT,

	created_at TEXT default '',
	retweet_count INTEGER default 0,
	favorite_count INTEGER default 0,

	tweet TEXT default '',			--deserialized python tweet object (for backup)
	primary key (tid),
	foreign key (usr_id)
	  references users (usr_id)
);
--test data
--insert into users (usr_id, name) values (1, "ruyk");
--insert into users (usr_id, name) values (2, "dbg");
--insert into tweets (tid, usr_id, text) values (1, 1, "first tweet");

--show data
--select * from users;
--select * from tweets;
