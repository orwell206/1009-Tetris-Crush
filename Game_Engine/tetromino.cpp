#include <initializer_list>
#pragma once



class Tetromino {
public:
	u8 *matrix;
	s32 side;

	Tetromino(u8 *matrix, s32 side){
		this->matrix = matrix;
		this->side = side;
	}
};


u8 TETROMINO_1[] = {
	0, 0, 0, 0,
	1, 1, 1, 1, 
	0, 0, 0, 0, 
	0, 0, 0, 0
};

u8 TETROMINO_2[] = {
	2, 2,
	2, 2
};

u8 TETROMINO_3[] = {
	0, 0, 0,
	3, 3, 3,
	0, 3, 0
};


u8 TETROMINO_4[] = {
	0, 4, 4,
	4, 4, 0,
	0, 0, 0
};


u8 TETROMINO_5[] = {
	5, 5, 0,
	0, 5, 5,
	0, 0, 0
};


u8 TETROMINO_6[] = {
	6, 0, 0,
	6, 6, 6,
	0, 0, 0
};


u8 TETROMINO_7[] = {
	0, 0, 7,
	7, 7, 7,
	0, 0, 0
};


Tetromino TETROMINOS[] = {
	Tetromino(TETROMINO_1, 4),
	Tetromino(TETROMINO_2, 2),
	Tetromino(TETROMINO_3, 3),
	Tetromino(TETROMINO_4, 3),
	Tetromino(TETROMINO_5, 3),
	Tetromino(TETROMINO_6, 3),
	Tetromino(TETROMINO_7, 3),
};