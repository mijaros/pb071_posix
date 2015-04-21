CC=gcc
CFLAGS=-std=c99

all: posix

posix: main.o
	$(CC) -o posix main.o
	
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f posix *.o

.PHONY: all clean
