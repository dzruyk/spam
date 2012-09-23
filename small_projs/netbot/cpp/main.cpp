#include <stdio.h>

#include "tcp_client.hpp"

#define TARGET_IP
#define TARGET_PORT

int
main(int argc, char *argv[])
{
	BOT *bot;

	bot = new BOT();

/*
	bot->set_ip(TARGET_IP);
	bot->set_port(TARGET_PORT);
	
	bot->init_connection(bot);

	
	bot->close_connection(bot);
*/
	return 0;

err:
	return 1;
}

