#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#define TEST_MSG "test_msg"

int 
main(int argc, const char *argv[])
{
	int fd;

	fd = open("./test.txt", O_WRONLY | O_CREAT, S_IRWXU);
	if (fd == -1)
		goto err;
	
	if (write(fd, TEST_MSG, strlen(TEST_MSG)) == -1)
		goto err;

	close(fd);

	return 0;
err:
	perror("");
	exit(1);
}
