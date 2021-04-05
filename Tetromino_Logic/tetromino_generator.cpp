#pragma once
#include "tetromino.cpp"

#define TETROMINO_COUNT 7

struct TetrominoGenerator {
private:
	int tetromino_shape_count = TETROMINO_COUNT;									// How many types of tetromino are there 
	u8 TETROMINO_I_SHAPE[16] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};	// Matrix for the "I" shape 
	u8 TETROMINO_T_SHAPE[9] = {0, 0, 0, 3, 3, 3, 0, 3, 0};							// Matrix for the "T" shape 
	u8 TETROMINO_L_SHAPE[9] = {0, 0, 7, 7, 7, 7, 0, 0, 0};							// Matrix for the "L" shape
	u8 TETROMINO_J_SHAPE[9] = {6, 0, 0, 6, 6, 6, 0, 0, 0};							// Matrix for the 'J' shape 
	u8 TETROMINO_S_SHAPE[9] = {0, 4, 4, 4, 4, 0, 0, 0, 0};							// Matrix for the "S" shape
	u8 TETROMINO_Z_SHAPE[9] = {5, 5, 0, 0, 5, 5, 0, 0, 0};							// Matrix for the "Z" shape 
	u8 TETROMINO_SQ_SHAPE[4] = {2, 2, 2, 2};										// Matrix for the "SQ" shape 
	Tetromino tetromino_list[TETROMINO_COUNT] = {									// Array containing the objects of the different tetromino types 
		Tetromino(TETROMINO_I_SHAPE, 4),
		Tetromino(TETROMINO_T_SHAPE, 3),
		Tetromino(TETROMINO_L_SHAPE, 3),
		Tetromino(TETROMINO_J_SHAPE, 3),
		Tetromino(TETROMINO_S_SHAPE, 3),
		Tetromino(TETROMINO_Z_SHAPE, 3),
		Tetromino(TETROMINO_SQ_SHAPE, 2)
	};
	const char *tetromino_names[7] = {												// Array containing the names of the objects of the different tetromino types 
		"TETROMINO_I_SHAPE",
		"TETROMINO_T_SHAPE",
		"TETROMINO_L_SHAPE",
		"TETROMINO_J_SHAPE",
		"TETROMINO_S_SHAPE",
		"TETROMINO_Z_SHAPE",
		"TETROMINO_SQ_SHAPE"
	};

public:
	int get_TetrominoShapeCount();													// Returns the number of the tetromino type.
	Tetromino *get_TetrominoList();													// Returns the array of tetromino objects. 
	const char *get_TetrominoName(int i);											// Returns the name of the tetromino type.

};

int TetrominoGenerator::get_TetrominoShapeCount() 
{ 
	/*
		Desc: Returns the number of the tetromino type. 
		
		Params: None. 
	*/
	return tetromino_shape_count; 
};		

Tetromino *TetrominoGenerator::get_TetrominoList() 
{ 
	/*
		Desc: Returns the array of tetromino objects.  
		
		Params: None. 
	*/
	return tetromino_list; 
}; 				

const char *TetrominoGenerator::get_TetrominoName(int i) 
{ 
	/*
		Desc: Returns the name of the tetromino type.
		
		Params: int (i). 
	*/
	return tetromino_names[i]; 
};	