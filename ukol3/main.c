#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int print_file(const char *path)
{
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    perror(path);
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

int print_stats(const char *path)
{
  struct stat st;
  if (stat(path, &st)) {
      perror(path);
      return -1;
  }

  printf("Statistiky souboru %s:\n", path);

  printf("velikost souboru: %lld\n", (long long) st.st_size);
  printf("posledni modifikace: %s", ctime(&(st.st_atime)));
  printf("UID majitele: %d\n", (int) st.st_uid);
  printf("GID majitele: %d\n\n\n", (int) st.st_gid);

  return 0;
}

int main(int argc, char *argv[])
{
  if (argc <= 2) {
    fprintf(stderr, "Invalid amount of arguments\n");
    return -1;
  }
  int (*op)(const char *) = NULL;

  if (strcmp(argv[1], "-p") == 0) {
      op = print_file;
  } else if (strcmp(argv[1], "-s") == 0) {
      op = print_stats;
  } else {
      fprintf(stderr, "Invalid argument %s\n", argv[1]);
      return -1;
  }

  for (int i = 2; i < argc; i++) {
      op(argv[i]);
  }

  return 0;
}

