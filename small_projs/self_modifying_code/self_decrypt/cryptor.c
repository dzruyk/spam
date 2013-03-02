#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>

#include "macros.h"

#define BUF_SZ 1024

void
print_usage(char *pname)
{
        printf(
	"USAGE: %s -o -k -t -h FILE\n"
	"-o offset\toffset from begining of file (hexademical)\n"
	"-t type\t\ttype of encrytion (now only XOR is supported)\n"
	"-k key\tkey for encryption (hexademical character)\n"
	"-l sz\tlen of encryption\n"
	, pname);
}

int
write_full(int fd, void *buf, size_t count)
{
	int writed;

	while (count > 0) {
		writed = write(fd, buf, count);
		if (writed == -1 && errno != EINTR)
			return 1;
		if (writed == 0)
			break;
		count -= writed;
		buf += count; 
	}
	
	return 0;
}

int
main(int argc, char *argv[])
{
	int fd;
	unsigned char key;
	int i;
	int len;
	int opt;
	int offset;
	char *pname;

	pname = argv[0];
	
	len = offset = 0;
	while ((opt = getopt(argc, argv, "o:t:k:l:h")) != -1) {
		switch (opt) {
		case 'o':
			errno = 0;
			offset = strtol(optarg, NULL, 16);
			if (errno) {
				printf("argument is not number\n");
				goto error;
			}
			break;
		case 't':
			break;
		case 'k':
			key = strtol(optarg, NULL, 16);
			if (errno) {
				printf("argument is not number\n");
				goto error;
			}
			break;
		case 'l':
			len = strtol(optarg, NULL, 16);
			if (errno) {
				printf("argument is not number\n");
				goto error;
			}
			break;
		case 'h':
			print_usage(pname);
			return 0;
		default:
			goto error;
			break;
		}
	}
	argc -= optind;
	argv += optind;
	
	if (argc < 1) {
		print_usage(pname);
		exit(1);
	}

	fd = open(argv[0], O_RDWR | O_CLOEXEC);
	if (fd == -1) {
		perror("open");
		exit(1);
	}
	if (lseek(fd, offset, SEEK_SET) == -1) {
		perror("seek error:");
		exit(1);
	}

	
	while (len > 0) {
		char buf[BUF_SZ];
		int n;

		printf("step\n");

		n = read(fd, buf, BUF_SZ);
		if (n == -1) {
			perror("read");
			exit(1);
		} else if (n == 0) {
			warning("eof, but %d bytes remaining\n", len);
			break;
		}
	
		for (i = 0; i < n && i < len; i++)
			buf[i] ^= key;
		
		if (lseek(fd, -n, SEEK_CUR) == -1) {
			perror("seek err");
			exit(1);
		}

		if (write_full(fd, buf, i) != 0) {
			perror("write");
			break;
		}
		len -= i;
	}

	return 0;

error:
	print_usage(pname);
	exit(1);

}
