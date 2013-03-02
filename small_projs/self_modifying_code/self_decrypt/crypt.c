#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <sys/mman.h>
#include <unistd.h>

#define hello_sz ((int)hello_end - (int)print_hello)

asm (".string \"google\"\n"
     "nop;nop;nop;nop;nop;nop;nop;nop;nop;");
void
print_hello()
{
	char *s = "hello, world";
	printf(s);
}
asm (".string \"google\"");

void hello_end () {}


void
crypt_it(void *ptr, int sz, char mask)
{
	void *page;
	char *sp = ptr;
	int pagesize;
	int len;

	pagesize = sysconf(_SC_PAGE_SIZE);

	page = ptr - (((int)ptr) % pagesize);

	len = sz + ((int )ptr) % pagesize;
	if (mprotect(page, len, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
		perror("mprotect");
		exit(1);
	}

	while (sz--)
		*sp++ ^= mask;

	if (mprotect(page, len, PROT_READ | PROT_EXEC) != 0) {
		perror("mprotect");
		exit(1);
	}
}

int
main()
{
	//decrypt
	crypt_it(print_hello, hello_sz, 0xab);

	print_hello();

	//crypt again
	crypt_it(print_hello, hello_sz, 0xab);

	return 0;
}

