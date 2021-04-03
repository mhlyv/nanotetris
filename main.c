#include <stdio.h>
#include <stdint.h>

#include "tetris.h"

static inline void printBoard() {
	for (uint8_t i = 0; i < BOARD_H; i++) {
		for (uint8_t j = 0; j < BOARD_W; j++) {
			printf("%u", getBoardBlock(j, i));
		}
		putchar('\n');
	}
}

static inline void printTetromino(uint8_t t) {
	// get the size from the first bit
	uint8_t size = 3 + (tetris.tetrominos[t] >> 15);

	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
			putchar(getTetrominoBlock(t, j, i) ? '1' : '0');
		}
		putchar('\n');
	}
}

int main() {
	init();

	tetris.tetromino = 4;
	tetris.x = 3;
	tetris.y = 1;
	printBoard();

	for (uint8_t i = 0; i < 7; i++) {
		puts("----------------");
		printTetromino(i);
		puts("----------------");
	}

	for (uint8_t i = 0; i < 7; i++) {
		rotateTetrominoCW(i);
	}

	for (uint8_t i = 0; i < 7; i++) {
		puts("----------------");
		printTetromino(i);
		puts("----------------");
	}

	printBoard();

	for (uint8_t i = 0; i < 7; i++) {
		rotateTetrominoCCW(i);
		rotateTetrominoCCW(i);
	}

	for (uint8_t i = 0; i < 7; i++) {
		puts("----------------");
		printTetromino(i);
		puts("----------------");
	}

	printBoard();
	return 0;
}
