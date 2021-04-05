all: main

CFLAGS=-Wall -Wextra -Wpedantic -g

main: main.c tetris.h
	$(CC) $(CFLAGS) main.c
