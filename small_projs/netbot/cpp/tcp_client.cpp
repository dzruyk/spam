#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>


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
	
	return true;
}

bool BOT::close_connection()
{
	DEBUG(LOG_VERBOSE, "bot close connection\n");
	
	return true;
}


