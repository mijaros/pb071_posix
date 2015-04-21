#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Invalid amount of arguments\n");
    return -1;
  }
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    perror(argv[1]);
    return -1;
  }

  char buffer[1024] = {'\0'};
  size_t size = 0;
  while ( (size = fread(buffer, 1, 1024, file)) > 0) {
    fwrite(buffer, 1, size, stdout);
  }

  fclose(file);
  return 0;
}
