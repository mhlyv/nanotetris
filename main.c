#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tetris.h"

uint8_t tetris_random() {
	return rand();
}

static inline void print_board() {
	for (uint8_t i = 0; i < BOARD_H; i++) {
		for (uint8_t j = 0; j < BOARD_W; j++) {
			putchar(tetris_get_board_block(j, i) ? '#' : '.');
			putchar(' ');
		}
		putchar('\n');
	}
}

static inline void print_tetromino(uint8_t t) {
	// get the size from the first bit
	uint8_t size = 3 + (tetris.tetrominos[t] >> 15);

	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
			putchar(tetris_get_tetromino_block(t, j, i) ? '#' : '.');
			putchar(' ');
		}
		putchar('\n');
	}
}

int main() {
	srand(time(NULL));
	tetris_init();

	while (1) {
		putchar('\n');
		print_board();
		printf("> ");
		fflush(stdout);
		char c;
		scanf("%c", &c);
		switch (c) {
			case 'q':
				return 0;
				break;
			case 'h':
				tetris_move_tetromino_left();
				break;
			case 'j':
				tetris_rotate_ccw();
				break;
			case 'k':
				tetris_rotate_cw();
				break;
			case 'l':
				tetris_move_tetromino_right();
				break;
			default:
				break;
		}
		tetris_update();
		print_tetris();
	}

	print_board();
	return 0;
}
