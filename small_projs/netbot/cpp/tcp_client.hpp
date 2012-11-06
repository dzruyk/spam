#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "sock.hpp"

class BOT 
{
public:
	BOT();
	bool set_ip(char *serv_ip);
	bool set_port(char *serv_port);

	bool init_connection();
	bool close_connection();

	bool main_loop();
	bool send_client_info();

	int get_next_cmd(struct command_ctx *ctx);
	bool get_cli_mode();
private:
	bool hand_shake();
	bool validate_server(char *msg, int sz);

	class Socket *socket;
};


#endif
