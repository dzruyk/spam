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

	if (kill(pid, SIGSTOP) != 0)
		print_warn("can't kill\n");

	errno = 0;
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
	       regs.eax, regs.ebx, regs.ecx,
	       regs.edx,
	       regs.orig_eax,
	       regs.eip);
}

void
ptrace_cur_instruction()
{

}

void
get_signal(pid_t pid)
{
	int ret, st;

	ret = waitpid(pid, &st, WNOHANG);
	if (ret == -1) {
		print_warn("wait pid error\n");
		return;
	}
	if (ret == 0)
		return;

	if (WIFEXITED(st)) 
		DEBUG(LOG_DEFAULT, "child exited with status %d\n", WEXITSTATUS(st));
	else if (WIFSIGNALED(st))
		DEBUG(LOG_DEFAULT, "child signalled with signal %d\n", WTERMSIG(st));
	else if (WIFSTOPPED(st))
		DEBUG(LOG_DEFAULT, "child stopped\n");
	else if (WIFCONTINUED(st))
		DEBUG(LOG_DEFAULT, "child continued\n");
}

int
main(int argc, char *argv[])
{
	int ret, child;
	char answer[BUFLEN];

	child = ptrace_init(CHILD_NAME);

	DEBUG(LOG_DEFAULT, "child pid is %d\n", child);

	while (1) {
		ptrace_print_regs(child);
		ret = ptrace_next_step(child);
		ptrace_print_regs(child);
		if (ret != 0)
			break;
		
		printf("next?\n");
		
		memset(answer, 0, BUFLEN);
		fgets(answer, BUFLEN, stdin);
		if (strcmp(answer, "n\n") == 0)
			break;

	}
	
	//FIXME: need to check is child allive
	kill(child, SIGKILL);

	return 0;
}

