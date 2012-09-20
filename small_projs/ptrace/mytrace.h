#ifndef __MYTRACE_H__
#define __MYTRACE_H__

enum request {
	MYTRACE_TRACEME,
	MYTRACE_GETREGS,
	MYTRACE_SETREGS,
	MYTRACE_GETFPREGS,
	MYTRACE_SETFPREGS,

	MYTRACE_CONTINUE,
	MYTRACE_KILL,
	MYTRACE_ATTACH,

	MYTRACE_SINGLESTEP,
};


int mytrace(request req, pid_t pid, void *addr, void *data);

#endif
