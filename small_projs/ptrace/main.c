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

void
ptrace_step(pid_t pid)
{
	ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
}

void
ptrace_print_regs(pid_t pid)
{
	struct user_regs_struct regs;

	ptrace(PTRACE_GETREGS, pid, NULL, &regs);

	printf("eax = %.8x\t ebx = %.8x\t ecx = %.8x\n"
	       "edx = %.8x\t\n"
	       "orig_eax = %.8x\n" 
	       "eip = %.8x\n",
	       regs.eax, regs.ebx, regs.ecx,
	       regs.edx,
	       regs.orig_eax,
	       regs.eip);
}

void
ptrace_cur_instruction()
{

}

int
main(int argc, char *argv[])
{
	int i, child;
	char answer[BUFLEN];

	child = ptrace_init(CHILD_NAME);


	while (1) {
		ptrace_print_regs(child);
		ptrace_step(child);

		printf("next?\n");
		
		memset(answer, 0, BUFLEN);
		fgets(answer, BUFLEN, stdin);
		if (strcmp(answer, "n\n") == 0)
			break;
	}
	
	kill(child, SIGKILL);

	return 0;
}

