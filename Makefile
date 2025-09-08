CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -O2
ifdef DEBUG
CFLAGS += -DDEBUG
endif

a1: clean main.c
	$(CC) $(CFLAGS) main.c -o a1

.PHONY: clean debug
debug: clean
	$(MAKE) DEBUG=1 a1

clean:
	rm -f a1
