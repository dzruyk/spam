#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <sys/mman.h>
#include <unistd.h>

#define SZ ((int)func_end - (int)func)

extern int func(int, int);

asm(
".globl  func 				\n"
".type   func, @function		\n"
"func:					\n"
".cfi_startproc				\n"
"movl 4(%esp), %eax			\n"
"call .1				\n"
".1: 					\n"
"addl 12(%esp), %eax			\n"
"popl %edx				\n"
"xorl $0x28, (%edx)			\n"
"ret					\n"
".cfi_endproc				\n"
);

void func_end () {}

int
main()
{
	int i;
	int pagesize;
	void *page;
	
	pagesize = sysconf(_SC_PAGE_SIZE);

	page = func - (((int)func) % pagesize);
	if (mprotect(page, SZ + ((int)func) % pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
		perror("mprotect");

	for (i = 0; i < 10; i++)
		printf ("%d) %d\n", i, func(2, 4));

	return 0;
}

