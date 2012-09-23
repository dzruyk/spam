#include <stdio.h>

#include "tcp_client.hpp"
#include "log.h"

 BOT::BOT()
{
	DEBUG(LOG_VERBOSE, "bot initialisation\n");

	this->sock = 0;
	this->serv_ip = this->serv_port = 
	    this->client_ip = this->client_port = 0;
}

