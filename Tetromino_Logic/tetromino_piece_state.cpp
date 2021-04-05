#include "../common_data.hpp"
#pragma once

class TetrominoPieceState{
	private:
		u8 tetromino_index;								// Tetromino index based on tetromino_generator.cpp
		s32 offset_row;									// Coordinate offset for drawing on board
		s32 offset_col;									// Coordinate offset for drawing on board
		s32 rotation;									// Rotational state
	public:
		void resetState();								// Reset values of the tetromino state for a new tetromino block.
		u8 get_tetromino_index() const;					// Gets the tetromino index.
		void set_tetromino_index(u8 tetromino_index);	// Sets the tetromino index.
		s32 get_offset_row() const;						// Gets the offset row of the tetromino.
		void set_offset_row(s32 offset_row);			// Set coordinate row offset
		s32 get_offset_col() const;						// Get coordinate col offset
		void set_offset_col(s32 offset_row);			// Set coordinate col offset
		s32 get_rotation() const;						// Gets the rotational state of the tetromino.
		void set_rotation(s32 offset_row);				// Sets the rotational state of the tetromino.
};

void TetrominoPieceState::resetState() {
	/*
		Desc: Reset values of the tetromino state for a new tetromino block.

		Params: None.
	*/
	this->tetromino_index = 0;
	this->offset_row = 0;
	this->offset_col = 0;
	this->rotation = 0;
}

u8 TetrominoPieceState::get_tetromino_index() const
{
	/*
		Desc: Gets the tetromino index.

		Params: None.
	*/
	return tetromino_index;
}

void TetrominoPieceState::set_tetromino_index(u8 tetromino_index)
{
	/*
		Desc: Sets the tetromino index.

		Params: u8 (tetromino_index).
	*/
	this->tetromino_index = tetromino_index;
}

s32 TetrominoPieceState::get_offset_row() const
{
	/*
		Desc: Gets the offset row of the tetromino.

		Params: None.
	*/
	return offset_row;
}

void TetrominoPieceState::set_offset_row(s32 offset_row)
{
	/*
		Desc: Sets the offset row of the tetromino.

		Params: s32 (offset_row).
	*/
	this->offset_row = offset_row;
}

s32 TetrominoPieceState::get_offset_col() const
{
	/*
		Desc: Gets the offset column of the tetromino.

		Params: None.
	*/
	return offset_col;
}

void TetrominoPieceState::set_offset_col(s32 offset_col)
{
	/*
		Desc: Sets the offset row of the tetromino.

		Params: s32 (offset_col).
	*/
	this->offset_col = offset_col;
}

s32 TetrominoPieceState::get_rotation() const
{
	/*
		Desc: Gets the rotational state of the tetromino.

		Params: None.
	*/
	return rotation;
}

void TetrominoPieceState::set_rotation(s32 rotation)
{
	/*
		Desc: Sets the rotational state of the tetromino.

		Params: s32 (offset_row).
	*/
	this->rotation = rotation;
}
