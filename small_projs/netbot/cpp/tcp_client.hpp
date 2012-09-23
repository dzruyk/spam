#ifndef _CLIENT_H_
#define _CLIENT_H_


class BOT 
{
public:
	BOT();
	bool set_ip(char *serv_ip);
	bool set_port(char *serv_port);

	bool init_connection();

	bool close_connection();

private:
	int sock;
	
	int serv_ip;
	int serv_port;
	
	int client_ip;
	int client_port;
};


#endif
