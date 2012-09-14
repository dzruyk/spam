#include <errno.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>

#include "common.h"
#include "log.h"

#define CHILD_NAME "./_test"

enum {
	BUFLEN = 128,
};

enum proc_st {
	ST_ERR = 0,
	ST_SAME,	// No state changes
	ST_EXIT,	// proc exited
	ST_SIGNAL,	// proc signalled
	ST_STOP,	// stopped
	ST_CONTINUE,	// proc continued
};

/*
 * Process state structure.
 * Used to retrieve info wia waitpid
 * st holds one of allowed process states
 * helper holds some additional info:
 * return status if ST_EXIT occurred,
 * signal number if ST_SIGNAL,
 */
struct proc_state {
	enum proc_st	st;
	int helper;
};

int get_state(pid_t pid, struct proc_state *pstate);

/*
 * Get pid of traced process or die.
 */
pid_t
ptrace_init(char *pname)
{
	int pid, res, status;

	pid = fork();

	switch (pid) {
	case 0:
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		
		res = execl(pname, pname, NULL);
		if (res == -1)
			print_warn_and_die("execl_return fail\n");

	case -1:
		perror("fork ");
		exit(1);
	default:
		break;
	}

	while (1) {
		res = waitpid(-1, &status, WNOHANG);
		switch (res) {
		case 0:
			DEBUG(LOG_VERBOSE, "No waiting processes, idle\n");
			usleep(250);
			continue;
		case -1:
			print_warn("waitpid error\n");
			exit(1);
		default:
			DEBUG(LOG_VERBOSE, "[+] waitpid return %d\n", res);
			if (WIFSTOPPED(status))
				DEBUG(LOG_VERBOSE, "[+] Process was stopped, seems like child traced\n");
			if (WIFEXITED(status)) {
				DEBUG(LOG_VERBOSE, "[-] Unexpected exit, seems like child have execve error\n");
				exit(1);
			}

			return res;
		}
	}
}

/*
 * Try to execute next instruction.
 * returns 0 if success
 * returns 1 otherwise.
 */
int
ptrace_next_step(pid_t pid)
{
	int ret;
	
	ret = ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
	if (ret == -1) {
		perror("ptrace_next_step ");
		return 1;
	}
	return 0;
}

void
ptrace_print_regs(pid_t pid)
{
	int ret;
	static struct user_regs_struct regs;

	ret = ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	if (ret == -1) {
		perror("ptrace_print_regs");
		printf("pid is %d\n", pid);
		return;
	}

	printf("eax = %.8x\t ebx = %.8x\t ecx = %.8x\n"
	       "edx = %.8x\t\n"
	       "orig_eax = %.8x\n" 
	       "eip = %.8x\n",
	       (int)regs.eax, (int)regs.ebx, (int)regs.ecx,
	       (int)regs.edx,
	       (int)regs.orig_eax,
	       (int)regs.eip);
}

void
ptrace_cur_instruction()
{

}


int
get_state(pid_t pid, struct proc_state *pstate)
{
	int ret, st;

	ret = waitpid(pid, &st, WNOHANG);
	if (ret == -1) {
		DEBUG(LOG_DEFAULT, "wait pid error\n");
		pstate->st = ST_ERR;
		return ST_ERR;
	}
	if (ret == 0)
		return ST_SAME;

	if (WIFEXITED(st)) {
		pstate->st = ST_EXIT;
		pstate->helper = WEXITSTATUS(st);
		DEBUG(LOG_DEFAULT, "child exited with status %d\n", pstate->helper);
	} else if (WIFSIGNALED(st)) {
		pstate->st = ST_SIGNAL;
		pstate->helper = WTERMSIG(st);
		DEBUG(LOG_DEFAULT, "child signalled with signal %d\n", pstate->helper);
	} else if (WIFSTOPPED(st)) {
		pstate->st = ST_STOP;
		DEBUG(LOG_DEFAULT, "child stopped\n");
	} else if (WIFCONTINUED(st)) {
		pstate->st = ST_CONTINUE;
		DEBUG(LOG_DEFAULT, "child continued\n");
	}

	return pstate->st;
}

int
main(int argc, char *argv[])
{
	struct proc_state pstate;
	int ret, child;
	char answer[BUFLEN];

	child = ptrace_init(CHILD_NAME);

	DEBUG(LOG_DEFAULT, "child pid is %d\n", child);

	while (1) {
		DEBUG(LOG_VERBOSE, "new iter started\n");

		ptrace_print_regs(child);

		ret = ptrace_next_step(child);
		if (ret != 0)
			break;

		printf("next?\n");
		
		memset(answer, 0, BUFLEN);
		fgets(answer, BUFLEN, stdin);
		if (strcmp(answer, "n\n") == 0)
			break;
		
		do {
			ret = get_state(child, &pstate);
			switch (ret) {
			case ST_ERR:
				print_warn_and_die("Can't get child state\n");
			case ST_SAME:
				continue;
			case ST_EXIT:
				goto finalize;
				break;
			case ST_STOP:
			case ST_SIGNAL:
			case ST_CONTINUE:
				break;
			}
		} while (ret == ST_SAME);


	}
	
	//FIXME: need to check is child allive
	if (child != -1)
		kill(child, SIGKILL);
finalize:

	return 0;
}

