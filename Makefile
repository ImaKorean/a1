CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -O2

a1: main.c
	$(CC) $(CFLAGS) main.c -o a1

clean:
	rm -f a1 *.o