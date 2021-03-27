#include "../common_data.hpp"
#pragma once


class TetrominoPieceState{
	public:
		u8 tetromino_index;
		s32 offset_row;
		s32 offset_col;
		s32 rotation;

		void resetState();
		u8 get_tetromino_index();
		void set_tetromino_index(u8 tetromino_index);
		s32 get_offset_row();
		void set_offset_row(s32 offset_row);
};

void TetrominoPieceState::resetState() {
	this->tetromino_index = 0;
	this->offset_row = 0;
	this->offset_col = 0;
	this->rotation = 0;
}