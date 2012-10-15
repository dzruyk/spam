#ifndef _MYSOCKET_H_
#define _MYSOCKET_H_

class Socket
{
public:
	Socket();
	bool create_socket();
	bool connect_socket();
	bool get_sock(int *sock);
	bool set_ip(char *serv_ip);
	bool set_port(char *serv_port);
	bool send_msg(char *msg, int len);
	bool recv_msg(char *msg, int *len, int maxlen);
	bool close_socket();

	int is_closed;
private:
	int sock;
	
	in_addr_t serv_ip;
	int serv_port;
	
	in_addr_t client_ip;
	int client_port;

	int sock_domain;
	int sock_type;
};

#endif
