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


struct TetrominoGenerator {
private:
	int tetromino_shape_count = 7;
	u8 TETROMINO_I_SHAPE[16] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
	u8 TETROMINO_T_SHAPE[9] = {0, 0, 0, 3, 3, 3, 0, 3, 0};
	u8 TETROMINO_L_SHAPE[9] = {0, 0, 7, 7, 7, 7, 0, 0, 0};
	u8 TETROMINO_J_SHAPE[9] = {6, 0, 0, 6, 6, 6, 0, 0, 0};
	u8 TETROMINO_S_SHAPE[9] = {0, 4, 4, 4, 4, 0, 0, 0, 0};
	u8 TETROMINO_Z_SHAPE[9] = {5, 5, 0, 0, 5, 5, 0, 0, 0};
	u8 TETROMINO_SQ_SHAPE[4] = {2, 2, 2, 2};
	Tetromino tetromino_list[7] = {
		Tetromino(TETROMINO_I_SHAPE, 4),
		Tetromino(TETROMINO_T_SHAPE, 3),
		Tetromino(TETROMINO_L_SHAPE, 3),
		Tetromino(TETROMINO_J_SHAPE, 3),
		Tetromino(TETROMINO_S_SHAPE, 3),
		Tetromino(TETROMINO_Z_SHAPE, 3),
		Tetromino(TETROMINO_SQ_SHAPE, 2)
	};
	const char *tetromino_names[7] = {
		"TETROMINO_I_SHAPE",
		"TETROMINO_T_SHAPE",
		"TETROMINO_L_SHAPE",
		"TETROMINO_J_SHAPE",
		"TETROMINO_S_SHAPE",
		"TETROMINO_Z_SHAPE",
		"TETROMINO_SQ_SHAPE"
	};
public:
	int get_TetrominoShapeCount() { return tetromino_shape_count; };
	Tetromino *get_TetrominoList() { return tetromino_list; };
	const char *get_TetrominoName(int i) { return tetromino_names[i]; }; 
};