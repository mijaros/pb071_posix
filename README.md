# Cvičení 10

## 1. úkol cat

```{C}
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
  while ( (size = read(fd, )) > 0) {
    write(fd, buffer, 1024);
  }
  
  close(fd);
  return 0;
}
```

## 2. Úkol Informace o souboru

```{C}
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

  struct stat st;
  fstat(fd, &st);
  printf("velikost souboru: %lld\n", (long long) st.st_size);
  printf("posledni modifikace: %s\n", ctime(&(st.st_atime)));
  printf("UID majitele: %d\n", (int) st.st_uid);
  printf("GID majitele: %d\n", (int) st.st_gid);

  close(fd);
  return 0;
}
```

## 3. Úkol Refactoring
```{C}
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
```
## 4. Úkol Výpis adresářů
```{C}
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
    printf("Skenovani adresare %s:\n", path);
    printf("--------------------------------\n");
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
    printf("--------------------------------\n");
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
      stat(argv[i], &st);
      if (S_ISDIR(st.st_mode)) {
          print_directory(argv[i], op);
      } else if (S_ISREG(st.st_mode)) {
          op(argv[i]);
      } else {
          printf("%s is not file or directory\n", argv[i]);
      }
  }

  return 0;
}
```
