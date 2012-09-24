#include <assert.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>


#include "commands.h"
#include "messages.h"
#include "tcp_client.hpp"
#include "log.h"

BOT::BOT()
{
	DEBUG(LOG_VERBOSE, "bot initialisation\n");

	this->sock = 0;
	this->serv_port = 
	    this->client_ip = this->client_port = 0;
	
	//some default values
	this->sock_domain = AF_INET;
	this->sock_type = SOCK_STREAM;
	this->serv_port = 0;
	this->serv_ip = 0;
}

bool BOT::set_ip(char *serv_ip)
{
	DEBUG(LOG_VERBOSE, "bot set IP\n");

	this->serv_ip = inet_addr(serv_ip);
	
	if (this->serv_ip == INADDR_NONE)
		return false;
	
	return true;
}

bool BOT::set_port(char *serv_port)
{
	DEBUG(LOG_VERBOSE, "bot set port\n");
	uint16_t port;
//FIXME:
	port = atoi(serv_port);

	this->serv_port = htons(port);
	
	return true;
}

bool BOT::init_connection()
{
	DEBUG(LOG_VERBOSE, "bot init connection\n");
	
	assert(this->serv_ip != 0 
	    && this->serv_port != 0);

	if (this->sock != 0)
		return false;
	
	if (this->create_socket() == false)
		return false;

	if (this->connect_socket() == false) {
		this->close_socket();
		return false;
	}

	return true;
}


bool BOT::main_loop()
{
	int cmd;

	if (this->hand_shake() == false)
		return false;

	if (this->send_client_info() == false)
		return false;
	
	cmd = CMD_BYE;
	do {

	} while (cmd != CMD_BYE);


	return true;
}

bool BOT::send_client_info()
{
	return true;
}

bool BOT::connect_socket()
{
	struct sockaddr_in addr;
	int ret;

	addr.sin_family = this->sock_domain;
	addr.sin_port = this->serv_port;
	addr.sin_addr.s_addr = this->serv_ip;

	ret = connect(this->sock, (const sockaddr *)&addr, sizeof(addr));

	if (ret == -1) {
		DEBUG(LOG_DEFAULT, "can't connect to server: %s\n", strerror(errno));
		return false;
	}
	DEBUG(LOG_VERBOSE, "bot connection success\n");

	return true;
}

bool BOT::hand_shake()
{
	assert(this->sock != 0);
	char buff[128];
	int len;
	bool ret;

	ret = this->send_msg(HELLO_MSG, HELLO_MSG_LEN);
	if (ret == false) {
		DEBUG(LOG_DEFAULT, "send_msg handshake fail\n");
		return false;
	}

	ret = this->recv_msg(buff, *len, sizeof(buff));
	
	if (ret == false) {
		DEBUG(LOG_DEFAULT, "send_msg handshake fail\n");
		return false;
	}

	return true;
}

bool BOT::send_msg(char *msg, int len)
{
	assert(this->sock != 0);
	int ret, sended;

	sended = 0;

	do {
		ret = send(this->sock, msg, len, 0);
		if (ret == -1)
			goto err;
		msg += ret;
	} while ((sended += ret) < len);

	return true;
err:
	return false;
}

bool BOT::recv_msg(char *msg, int *len, int maxlen)
{
	assert(this->sock != 0);
	int ret;

//FIXME:
	ret = recv(this->sock, msg, maxlen, 0);
	if (ret == -1)
		goto err;

	*len = ret;
	
	return true;
err:
	return false;
}

bool BOT::create_socket()
{
	int sock;

	sock = socket(this->sock_domain, this->sock_type, 0);
	
	if (sock == -1) {
		DEBUG(LOG_DEFAULT, "can't create socket\n");
		return false;
	}
	this->sock = sock;

	return true;
}

bool BOT::close_socket()
{
	assert(this->sock != 0);

	shutdown(this->sock, SHUT_RDWR);

	this->sock = 0;

	return true;
}

bool BOT::close_connection()
{
	DEBUG(LOG_VERBOSE, "bot close connection\n");

	return this->close_socket();
}


