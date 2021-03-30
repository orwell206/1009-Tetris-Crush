#pragma once
#include <initializer_list>


class Tetromino {
	public:
		u8 *matrix;
		s32 side;

	Tetromino(u8 *matrix, s32 side){
		this->matrix = matrix;
		this->side = side;
	}
};