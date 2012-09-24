#include <stdio.h>

#define TARGET_IP
#define TARGET_PORT

int
main(int argc, char *argv[])
{
	BOT *bot;

	bot = bot_create();

	bot->set_ip(bot, TARGET_IP);
	bot->set_port(bot, TARGET_PORT);
	
	bot->init_connection(bot);

	
	bot->close_connection(bot);

	return 0;

err:
	return 1;
}

