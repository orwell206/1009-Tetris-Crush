#include "common_data.hpp"
#pragma once


class TetrominoPieceState{
	public:
		u8 tetromino_index;
		s32 offset_row;
		s32 offset_col; 
		s32 rotation;
};