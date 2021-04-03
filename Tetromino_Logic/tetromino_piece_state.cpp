#include "../common_data.hpp"
#pragma once

class TetrominoPieceState{
	private:
		u8 tetromino_index;								// Tetromino index based on tetromino_generator.cpp
		s32 offset_row;									// Coordinate offset for drawing on board
		s32 offset_col;									// Coordinate offset for drawing on board
		s32 rotation;									// Rotational state
	public:
		void resetState();								// Resets values of TetrominoState for new block
		u8 get_tetromino_index() const;					// Get index
		void set_tetromino_index(u8 tetromino_index);	// Set Index
		s32 get_offset_row() const;						// Get coordinate row offset
		void set_offset_row(s32 offset_row);			// Set coordinate row offset
		s32 get_offset_col() const;						// Get coordinate col offset
		void set_offset_col(s32 offset_row);			// Set coordinate col offset
		s32 get_rotation() const;						// Get rotational state
		void set_rotation(s32 offset_row);				// Set rotational state
};

void TetrominoPieceState::resetState() {
	this->tetromino_index = 0;
	this->offset_row = 0;
	this->offset_col = 0;
	this->rotation = 0;
}

u8 TetrominoPieceState::get_tetromino_index() const { return tetromino_index; }
void TetrominoPieceState::set_tetromino_index(u8 tetromino_index) { this->tetromino_index = tetromino_index; }
s32 TetrominoPieceState::get_offset_row() const { return offset_row; }
void TetrominoPieceState::set_offset_row(s32 offset_row) { this->offset_row = offset_row; }
s32 TetrominoPieceState::get_offset_col() const { return offset_col; }
void TetrominoPieceState::set_offset_col(s32 offset_col) { this->offset_col = offset_col; }
s32 TetrominoPieceState::get_rotation() const { return rotation; }
void TetrominoPieceState::set_rotation(s32 rotation) { this->rotation = rotation; }
