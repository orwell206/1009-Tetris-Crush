#include "../common_data.hpp"
#pragma once

class TetrominoPieceState{
	private:
		u8 tetromino_index;
		s32 offset_row;
		s32 offset_col;
		s32 rotation;
	public:
		void resetState();
		u8 get_tetromino_index() const;
		void set_tetromino_index(u8 tetromino_index);
		s32 get_offset_row() const;
		void set_offset_row(s32 offset_row);
		s32 get_offset_col() const;
		void set_offset_col(s32 offset_row);
		s32 get_rotation() const;
		void set_rotation(s32 offset_row);
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