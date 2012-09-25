#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>


#include "log.h"
#include "sock.hpp"

Socket::Socket()
{
	this->sock = 0;
	this->serv_port = 
	    this->client_ip = this->client_port = 0;
	
	//some default values
	this->sock_domain = AF_INET;
	this->sock_type = SOCK_STREAM;
	this->serv_port = 0;
	this->serv_ip = 0;

	this->is_closed = 1;
}

bool Socket::create_socket()
{
	if (this->sock != 0) {
		DEBUG(LOG_DEFAULT, "Seems like socket already created\n");
		return false;
	}

	int sock;

	sock = socket(this->sock_domain, this->sock_type, 0);
	
	if (sock == -1) {
		DEBUG(LOG_DEFAULT, "can't create socket\n");
		return false;
	}
	this->sock = sock;
	this->is_closed = 0;

	return true;
}

bool Socket::set_ip(char *serv_ip)
{
	this->serv_ip = inet_addr(serv_ip);
	
	if (this->serv_ip == INADDR_NONE)
		return false;
	
	return true;
}

bool Socket::set_port(char *serv_port)
{
	DEBUG(LOG_VERBOSE, "bot set port\n");
	uint16_t port;
//FIXME:
	port = atoi(serv_port);

	this->serv_port = htons(port);
	
	return true;
}

bool Socket::connect_socket()
{
	struct sockaddr_in addr;
	int ret;

	if (this->serv_ip == 0 
	    && this->serv_port == 0) {
		DEBUG(LOG_VERBOSE, "serv port or sert ip undefined yet\n");
		return false;
	}

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


bool Socket::send_msg(char *msg, int len)
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

bool Socket::recv_msg(char *msg, int *len, int maxlen)
{
	DEBUG(LOG_VERBOSE, "recv_msg\n");

	assert(this->sock != 0);
	int ret;

//FIXME:
	ret = recv(this->sock, msg, maxlen, 0);
	if (ret == -1)
		goto err;

	if (ret == 0) {
		this->is_closed = 1;
		goto err;
	}

	*len = ret;
	
	return true;
err:
	DEBUG(LOG_VERBOSE, "recv return %d\n", ret);

	return false;
}

bool Socket::close_socket()
{
	assert(this->sock != 0);

	shutdown(this->sock, SHUT_RDWR);

	this->sock = 0;

	return true;
}


