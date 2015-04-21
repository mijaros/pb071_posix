#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

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
  printf("GID majitele: %d\n", (int) st.st_gid);

  return 0;
}

int print_directory(const char *path, int (*func)(const char *))
{
    struct dirent *dir = NULL;
    DIR *dir_holder = opendir(path);
    char buffer[1024] = {'\0'};
    strcpy(buffer, path);
    int len = strlen(buffer);

    if (dir_holder == NULL) {
        perror(path);
        return -1;
    }

    while ((dir = readdir(dir_holder)) != NULL) {
        buffer[len] = '\0';
        strcat(buffer, dir->d_name);
        struct stat st;
        if (stat(buffer, &st)) {
            perror(path);
            continue;
        }
        if (!S_ISREG(st.st_mode)) {
            printf("%s is not regular file\n", buffer);
            continue;
        }
        func(buffer);
    }
    closedir(dir_holder);
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
      struct stat st;
      if (stat(argv[i], &st)) {
        perror(argv[i]);
        continue;
      }
      if (S_ISDIR(st.st_mode)) {
          printf("Skenovani adresare %s:\n", argv[i]);
          printf("--------------------------------\n");
          print_directory(argv[i], op);
          printf("--------------------------------\n\n");
      } else if (S_ISREG(st.st_mode)) {
          op(argv[i]);
      } else {
          printf("%s is not file or directory\n", argv[i]);
      }
  }

  return 0;
}

