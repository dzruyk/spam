#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#define OPT_MAXSZ 1024

enum {
	CMD_UNKNOWN,
	CMD_ERR,
	CMD_NOP,
	CMD_BYE,
	CMD_SHUTDOWN,
	CMD_GET_INFO,
	CMD_GET_CLI,
	CMD_SLEEP,
	CMD_RECV_FILE,
	CMD_SEND_FILE,
};

struct commands {
	int id;
	char *name;
} commands[] = {
	{ CMD_NOP, "\n" }, 
	{ CMD_BYE, "bye-bye" },
	{ CMD_SHUTDOWN, "shutdown" }, 
	{ CMD_GET_INFO, "get_info" },
	{ CMD_GET_CLI, "get_cli" }, 
	{ CMD_SLEEP, "sleep_me" },
	{ CMD_RECV_FILE, "get_file" },
	{ CMD_SEND_FILE, "send_file" },
};

/*
 * Stub now, rewrite to binary package commands
#define PAYLOAD_SZ 1024

struct package {
	int version;
	int len;
	int flags;
	int cmd;
	char payload[PAYLOAD_SZ];
};
*/

struct command_ctx {
	int cmd;
	union {
		char path[OPT_MAXSZ];
		int val;
	};
};

#endif
