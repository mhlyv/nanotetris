all: main

CFLAGS=-Wall -Wextra -Wpedantic -g

test:
	$(CC) $(CFLAGS) -DTEST main.c

main: main.c tetris.h
	$(CC) $(CFLAGS) main.c
