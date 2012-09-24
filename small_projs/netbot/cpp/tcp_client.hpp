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

	bool main_loop();
	bool send_client_info();

private:

	bool create_socket();
	bool connect_socket();
	bool hand_shake();
	bool send_msg(char *msg, int len);
	bool recv_msg(char *msg, int *len, int maxlen);
	bool close_socket();

	int sock;
	
	in_addr_t serv_ip;
	int serv_port;
	
	in_addr_t client_ip;
	int client_port;

	int sock_domain;
	int sock_type;
};


#endif

