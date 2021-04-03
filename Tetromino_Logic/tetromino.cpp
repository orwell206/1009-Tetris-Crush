#pragma once
#include <initializer_list>


class Tetromino {
	public:
		u8 *matrix;		// Tetromino "shape" drawn in 2D matrix
		s32 side;		// Length of longest side

	Tetromino(u8 *matrix, s32 side){
		this->matrix = matrix;
		this->side = side;
	}
};
