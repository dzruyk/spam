#ifndef __COMMANDS_H__
#define __COMMANDS_H__

enum {
	CMD_UNKNOWN,
	CMD_ERR,
	CMD_BYE,
	CMD_SHUTDOWN,
	CMD_GET_INFO,
	CMD_GET_CLI,
	CMD_RECV_FILE,
	CMD_SEND_FILE,
};

struct command_ctx {
	int cmd;
	union {
		char *path;
		int val;
	};
};

#endif
