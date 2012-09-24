

typedef struct {
	int sock;
	
	int serv_ip;
	int serv_port;
	
	int client_ip;
	int client_port;

	/* methods */
	int (*set_ip)(struct bot bot, char *serv_ip);
} BOT;


int
bot_create()
{


}




