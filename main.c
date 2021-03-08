#include <stdio.h>
#include <stdint.h>

#define BOARD_W 10
#define BOARD_H 20

struct Tetris {
	// Tetris board represented as 200 bits.
	uint8_t board[(BOARD_W * BOARD_H) / 8];

	// Tetrominos represented as bits.
	// Order: I, O, T, J, L, S, Z.
	// The last bit is always 0, so instead the first bit represents
	// if the given tetromino is 3x3 (0) or 4x4 (1).
	uint16_t tetrominos[7];

	// The index of the currently active tetromino [0 .. 6].
	uint8_t tetromino;

	// Coordinates of the current active tetromino (top left corner).
	int8_t x;
	int8_t y;
} tetris;

static inline void init() {
	// empty board
	for (uint8_t i = 0; i < (BOARD_W * BOARD_H) / 8; i++) {
		tetris.board[i] = 0;
	}

	// I tetromino
	// 1 (4x4)
	// 0 0 0 0
	// 1 1 1 1
	// 0 0 0 0
	// 0 0 0
	// 0b1000011110000000 = 0x8780
	tetris.tetrominos[0] = 0x8780;

	// O tetromino
	// 1 (4x4) (it's 2x2 but it rotates the same way)
	// 0 0 0 0
	// 0 1 1 0
	// 0 1 1 0
	// 0 0 0
	// 0b1000001100110000 = 0x8330
	tetris.tetrominos[1] = 0x8330;

	// T tetromino
	// 0 (3x3)
	// 0 0 0
	// 1 1 1
	// 0 1 0
	//
	// 0 0 0 0 0 0
	// 0b0000111010000000 = 0xE80
	tetris.tetrominos[2] = 0xE80;

	// J tetromino
	// 0 (3x3)
	// 0 1 0
	// 0 1 0
	// 1 1 0
	//
	// 0 0 0 0 0 0
	// 0b0010010110000000 = 0x2580
	tetris.tetrominos[3] = 0x2580;

	// L tetromino
	// 0 (3x3)
	// 0 1 0
	// 0 1 0
	// 0 1 1
	//
	// 0 0 0 0 0 0
	// 0b0010010011000000 = 0x24C0
	tetris.tetrominos[4] = 0x24C0;

	// S tetromino
	// 0 (3x3)
	// 0 1 1
	// 1 1 0
	// 0 0 0
	//
	// 0 0 0 0 0 0
	// 0b0011110000000000 = 0x3C00
	tetris.tetrominos[5] = 0x3C00;

	// Z tetromino
	// 0 (3x3)
	// 0 0 0
	// 1 1 0
	// 0 1 1
	//
	// 0 0 0 0 0 0
	// 0b0000110011000000 = 0xCC0
	tetris.tetrominos[6] = 0xCC0;

	// These will be overwritten when choosing a random tetromino.
	tetris.tetromino = 0;
	tetris.x = 0;
	tetris.y = 0;
}

// return the block (bit) in the tetromino with index t at (x, y)
static uint8_t getTetrominoBlock(uint8_t t, uint8_t x, uint8_t y) {
	// get the size from the first bit
	uint8_t size = 3 + (tetris.tetrominos[t] >> 15);
	uint8_t bitindex = y * size + x;

	// handle out of bounds indexes
	return (x >= size || y >= size) ? 0 : ((tetris.tetrominos[t] >> (14 - bitindex)) & 1);
}

// return the block (bit) on the board at (x, y)
static uint8_t getBoardBlock(uint8_t x, uint8_t y) {
	uint8_t bitindex = (y * BOARD_W) + x;
	uint8_t tetromino_overlay = getTetrominoBlock(tetris.tetromino, x - tetris.x, y - tetris.y);
	return ((tetris.board[bitindex / 8] >> (7 - (bitindex % 8))) & 1) | tetromino_overlay;
}

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

static void rotateTetrominoCCW(uint8_t t) {
	// clear tetromino, but keep the size indicator
	uint16_t rotated = tetris.tetrominos[t] & (1 << 15);
	// get the size from the first bit
	uint8_t size = 3 + (tetris.tetrominos[t] >> 15);

	// rotate counter clockwise
	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
			rotated |= getTetrominoBlock(t, size - 1 - i, j) << (14 - i * size - j);
		}
	}

	tetris.tetrominos[t] = rotated;
}

static void rotateTetrominoCW(uint8_t t) {
	// clear tetromino, but keep the size indicator
	uint16_t rotated = tetris.tetrominos[t] & (1 << 15);
	// get the size from the first bit
	uint8_t size = 3 + (tetris.tetrominos[t] >> 15);

	// rotate clockwise
	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
			rotated |= getTetrominoBlock(t, i, size - 1 - j) << (14 - i * size - j);
		}
	}

	tetris.tetrominos[t] = rotated;
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
