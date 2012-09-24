#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "tcp_client.hpp"

#define TARGET_IP (char*)"192.168.2.1"
#define TARGET_PORT (char*)"2222"

int
main(int argc, char *argv[])
{
	BOT *bot;

	bot = new BOT();

	if (bot->set_ip(TARGET_IP) == false)
		goto err;

	if (bot->set_port(TARGET_PORT) == false)
		goto err;
	
	if (bot->init_connection() == false)
		goto err;

	
	bot->close_connection();

	delete bot;

	return 0;

err:
	delete bot;

	return 1;
}

