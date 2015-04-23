#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Invalid amount of arguments\n");
		return -1;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror(argv[1]);
		return -1;
	}

	char buffer[1024] = {'\0'};
	ssize_t size = 0;
	while ( (size = read(fd, buffer, 1024)) > 0) {
		write(STDOUT_FILENO, buffer, size);
	}

	close(fd);
	return 0;
}
