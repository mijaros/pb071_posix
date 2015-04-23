#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

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

	struct stat st;
	fstat(fd, &st);
	printf("velikost souboru: %lld\n", (long long) st.st_size);
	printf("posledni modifikace: %s\n", ctime(&(st.st_atime)));
	printf("UID majitele: %d\n", (int) st.st_uid);
	printf("GID majitele: %d\n", (int) st.st_gid);

	close(fd);
	return 0;
}

