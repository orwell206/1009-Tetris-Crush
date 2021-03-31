#pragma once
#include "../common_data.hpp"
#include "../Tetromino_Logic/tetromino_piece_state.cpp"
#include "../Tetromino_Logic/tetromino.cpp"
#include "gameboard_graphics.cpp"
#include "../Player_Engine/input_state.cpp"
#include "../Player_Engine/player.cpp"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <list>

const u8 FRAMES_PER_DROP[] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};
const f32 TARGET_SECONDS_PER_FRAME = 1.f / 60.f;

class GameBoard : public GameBoardGraphics
{
private:
	u8 gameboard[WIDTH * HEIGHT];
	u8 lines[HEIGHT];
	s32 pending_line_count;
	Player *player;
	Game_Phase gamePhase;
	f32 nextDropTime;
	f32 highlighted_end_time;
	f32 time;
	s32 level;
	s32 cleared_lines;
	s32 line_count;
	s32 points;

public:
	f32 get_GameBoardTime();
	void set_GameBoardTime(f32);
	void set_player(Player &);
	void render_clearline_graphics(const GameBoard *, SDL_Renderer *);
	void render_game(const GameBoard *, SDL_Renderer *, TTF_Font *);
	inline u8 check_if_row_filled(const u8 *, s32, s32);
	inline u8 check_if_row_empty(const u8 *, s32, s32);
	s32 find_lines(const u8 *, s32, s32, u8 *);
	void clear_lines(u8 *, s32, s32, const u8 *);
	int update_game(GameBoard *, const InputState *, SDL_Renderer *, TTF_Font *);
	bool check_tetromino_valid(const TetrominoPieceState *, const u8 *, s32, s32);
	void spawn_tetromino(GameBoard *);
	void merge_tetrimino_on_board(GameBoard *);
	inline bool drop_tetromino(GameBoard *);
	inline f32 get_time_to_next_tetromino_drop(s32);
	void update_gameline(GameBoard *);
	void update_gameplay(GameBoard *, const InputState *);
	int game_Over(GameBoard *, SDL_Renderer *, TTF_Font *);
};

f32 GameBoard::get_GameBoardTime() { return time; }

void GameBoard::set_GameBoardTime(f32 time) { this->time = time; }

void GameBoard::set_player(Player &player) { this->player = &player; }

void GameBoard::render_clearline_graphics(const GameBoard *gameboard, SDL_Renderer *renderer)
{
	Color highlight_color = Color(0xFF, 0xFF, 0xFF, 0xFF);
	if (gameboard->gamePhase == GAME_PHASE_LINE)
	{
		for (s32 row = 0; row < HEIGHT; row++)
		{
			if (gameboard->lines[row])
			{
				s32 x = 0;
				s32 y = row * GRID_SIZE;
				fill_rect(renderer, x, y, WIDTH * GRID_SIZE, GRID_SIZE, highlight_color);
			}
		}
	}
}

void GameBoard::render_game(const GameBoard *gameboard, SDL_Renderer *renderer, TTF_Font *font)
{
	draw_on_board(renderer, gameboard->gameboard, WIDTH, HEIGHT, 0, 0);

	if (gameboard->gamePhase == GAME_PHASE_PLAY)
	{
		draw_tetromino(renderer, &gameboard->tetrominoPiece, 0, 0, false);
		TetrominoPieceState tetro_shadow = gameboard->tetrominoPiece;
		while (check_tetromino_valid(&tetro_shadow, gameboard->gameboard, WIDTH, HEIGHT))
		{
			// tetro_shadow.offset_row++;
			s32 temp = tetro_shadow.get_offset_row();
			tetro_shadow.set_offset_row(temp + 1);
		}
		// --tetro_shadow.offset_row;
		s32 temp = tetro_shadow.get_offset_row();
		tetro_shadow.set_offset_row(temp - 1);

		draw_tetromino(renderer, &tetro_shadow, 0, 0, true);
	}

	render_clearline_graphics(gameboard, renderer);

	//temp for rendering score text on GUI
	char buffer[4096];
	s32 x = TEXT_HORZ_ALIGN;
	s32 y = TEXT_VERT_ALIGN;
	Color highlight_color = Color(0xFF, 0xFF, 0xFF, 0xFF);
	draw_text(renderer, font, "Player Name:", x, y, TEXT_ALIGN_LEFT, highlight_color);
	draw_text(renderer, font, player->playTest.playerName.c_str(), x, y + 50, TEXT_ALIGN_LEFT, highlight_color);

	draw_text(renderer, font, "POINTS:", x, y + 100, TEXT_ALIGN_LEFT, highlight_color);

	snprintf(buffer, sizeof(buffer), "%d", gameboard->points);
	draw_text(renderer, font, buffer, x, (y + 150), TEXT_ALIGN_LEFT, highlight_color);

	draw_text(renderer, font, "NEXT BLOCK:", x, (y + 210), TEXT_ALIGN_LEFT, highlight_color);
}

inline u8 GameBoard::check_if_row_filled(const u8 *values, s32 width, s32 row)
{
	for (s32 col = 0; col < width; col++)
	{
		if (!get_matrix(values, width, row, col))
		{
			return 0;
		}
	}
	return 1;
}

inline u8 GameBoard::check_if_row_empty(const u8 *values, s32 width, s32 row)
{
	for (s32 col = 0; col < width; col++)
	{
		if (get_matrix(values, width, row, col))
		{
			return 0;
		}
	}
	return 1;
}

s32 GameBoard::find_lines(const u8 *values, s32 width, s32 height, u8 *lines_out)
{
	s32 count = 0;
	for (s32 row = 0; row < height; row++)
	{
		u8 filled = check_if_row_filled(values, width, row);
		lines_out[row] = filled;
		count += filled;
	}
	return count;
}

void GameBoard::clear_lines(u8 *values, s32 width, s32 height, const u8 *lines)
{
	s32 src_row = height - 1;
	for (s32 dst_row = height - 1; dst_row >= 0; dst_row--)
	{
		while (src_row > 0 && lines[src_row])
		{
			src_row--;
		}

		if (src_row < 0)
		{
			memset(values + dst_row * width, 0, width);
		}
		else
		{
			memcpy(values + dst_row * width, values + src_row * width, width);
			src_row--;
		}
	}
}

int GameBoard::update_game(GameBoard *gameboard, const InputState *input, SDL_Renderer *renderer, TTF_Font *font)
{
	switch (gameboard->gamePhase)
	{
	case GAME_PHASE_PLAY:
		update_gameplay(gameboard, input);
		break;

	case GAME_PHASE_LINE:
		update_gameline(gameboard);
		break;

	case GAME_PHASE_GAMEOVER:
		int returnValue = game_Over(gameboard, renderer, font);
		return returnValue;
	}
	return 2;
}

bool GameBoard::check_tetromino_valid(const TetrominoPieceState *tetromino_piece, const u8 *gameboard, s32 width, s32 height)
{
	const Tetromino *tetromino = current_tetromino;

	assert(tetromino);

	for (s32 row = 0; row < tetromino->side; row++)
	{
		for (s32 col = 0; col < tetromino->side; col++)
		{
			u8 value = get_tetromino(tetromino, row, col, tetromino_piece->get_rotation());
			if (value > 0)
			{
				// s32 board_row = tetromino_piece->offset_row + row;
				// s32 board_col = tetromino_piece->offset_col + col;
				s32 board_row = tetromino_piece->get_offset_row() + row;
				s32 board_col = tetromino_piece->get_offset_col() + col;
				if (board_row < 0)
				{
					return false;
				}
				if (board_row >= height)
				{
					return false;
				}
				if (board_col < 0)
				{
					return false;
				}
				if (board_col >= width)
				{
					return false;
				}
				if (get_matrix(gameboard, width, board_row, board_col))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void GameBoard::spawn_tetromino(GameBoard *gameboard)
{
	// Grabs "next tetromino" and preps it for spawn
	gameboard->tetrominoPiece.resetState();
	gameboard->tetrominoPiece.set_tetromino_index(next_tetromino_index);
	gameboard->tetrominoPiece.set_offset_col(WIDTH / 2);
	gameboard->nextDropTime = gameboard->time + get_time_to_next_tetromino_drop(gameboard->level);

	// Set "current tetromino" to the one prep'ed above
	current_tetromino = tetrominoStruct.get_TetrominoList() + gameboard->tetrominoPiece.get_tetromino_index();

	// Select new randon "next tetromino"
	next_tetromino_index = (u8)random_tetromino_index(0, tetrominoStruct.get_TetrominoShapeCount());
	next_tetromino = tetrominoStruct.get_TetrominoList() + next_tetromino_index;
}

void GameBoard::merge_tetrimino_on_board(GameBoard *gameboard)
{
	const Tetromino *tetromino = current_tetromino;

	for (s32 row = 0; row < tetromino->side; row++)
	{
		for (s32 col = 0; col < tetromino->side; col++)
		{
			u8 value = get_tetromino(tetromino, row, col, gameboard->tetrominoPiece.get_rotation());
			if (value)
			{
				// s32 board_row = gameboard->tetrominoPiece.offset_row + row;
				// s32 board_col = gameboard->tetrominoPiece.offset_col + col;
				s32 board_row = gameboard->tetrominoPiece.get_offset_row() + row;
				s32 board_col = gameboard->tetrominoPiece.get_offset_col() + col;
				set_matrix(gameboard->gameboard, WIDTH, board_row, board_col, value); // problem
			}
		}
	}
}

inline bool GameBoard::drop_tetromino(GameBoard *gameboard)
{
	// ++gameboard->tetrominoPiece.offset_row;
	s32 temp = gameboard->tetrominoPiece.get_offset_row();
	gameboard->tetrominoPiece.set_offset_row(temp + 1);
	if (!check_tetromino_valid(&gameboard->tetrominoPiece, gameboard->gameboard, WIDTH, HEIGHT))
	{
		// --gameboard->tetrominoPiece.offset_row;
		s32 temp = gameboard->tetrominoPiece.get_offset_row();
		gameboard->tetrominoPiece.set_offset_row(temp - 1);
		merge_tetrimino_on_board(gameboard);
		spawn_tetromino(gameboard);
		return false;
	}
	gameboard->nextDropTime = gameboard->time + get_time_to_next_tetromino_drop(gameboard->level);
	return true;
}

inline f32 GameBoard::get_time_to_next_tetromino_drop(s32 level)
{
	if (level > 29)
	{
		level = 29;
	}
	return FRAMES_PER_DROP[level] * TARGET_SECONDS_PER_FRAME;
}

void GameBoard::update_gameline(GameBoard *gameboard)
{
	if (gameboard->time >= gameboard->highlighted_end_time)
	{
		clear_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
		gameboard->line_count += gameboard->pending_line_count;
		//For debugging
		std::cout << gameboard->pending_line_count;
		//Temp adding of score
		gameboard->points += 20 * gameboard->pending_line_count;
		gameboard->gamePhase = GAME_PHASE_PLAY;
	}
}

void GameBoard::update_gameplay(GameBoard *gameboard, const InputState *input)
{

	TetrominoPieceState tetrominoPiece = gameboard->tetrominoPiece;
	if (input->get_da() > 1)
	{
		// --tetrominoPiece.offset_col;
		s32 temp = tetrominoPiece.get_offset_col();
		tetrominoPiece.set_offset_col(temp - 1);
	}
	if (input->get_dd() > 1)
	{
		// ++tetrominoPiece.offset_col;
		s32 temp = tetrominoPiece.get_offset_col();
		tetrominoPiece.set_offset_col(temp + 1);
	}
	if (input->get_dw() > 1)
	{
		// tetrominoPiece.rotation = (tetrominoPiece.rotation + 1) % 4;
		s32 temp = tetrominoPiece.get_rotation();
		tetrominoPiece.set_rotation((temp + 1) % 4);
	}

	if (check_tetromino_valid(&tetrominoPiece, gameboard->gameboard, WIDTH, HEIGHT))
	{
		gameboard->tetrominoPiece = tetrominoPiece;
	}

	if (input->get_ds() > 1)
	{
		drop_tetromino(gameboard);
	}

	if (input->get_dspace() > 1)
	{
		while (drop_tetromino(gameboard))
			;
	}

	while (gameboard->time >= gameboard->nextDropTime)
	{
		drop_tetromino(gameboard);
	}

	s32 line_count = find_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
	//temp for adding pending line count
	gameboard->pending_line_count = find_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
	if (line_count > 0)
	{
		gameboard->gamePhase = GAME_PHASE_LINE;
		gameboard->highlighted_end_time = gameboard->time + 0.5f;
	}

	s32 game_over_row = 0;

	if (!check_if_row_empty(gameboard->gameboard, WIDTH, game_over_row))
	{
		gameboard->gamePhase = GAME_PHASE_GAMEOVER;
	}
}

int GameBoard::game_Over(GameBoard *gameboard, SDL_Renderer *renderer, TTF_Font *font)
{

	int playerPoints = gameboard->points;
	int indexToInsert, currentIndex = 0;
	std::string str = "Continue or Exit?\nHighest Points: " + std::to_string(playerPoints) + "\nLeaderBoard: " + "\nName:Score";
	/* try to open file to read */
	std::ifstream ifile;
	ifile.open("scoreboard.txt");
	if (ifile)
	{
		std::string line;
		std::list<Player> fileInputPlayer;
		Player insertPlayer;

		while (getline(ifile, line, ','))
		{
			Player tempPlayer;
			tempPlayer.playTest.playerName = line;

			getline(ifile, line);

			int fileScore = stoi(line);
			std::string playerHistory = tempPlayer.playTest.playerName + ":" + std::to_string(fileScore);
			str += "\n" + playerHistory;
			tempPlayer.playTest.playerScore = fileScore;

			fileInputPlayer.push_back(tempPlayer);

			if (playerPoints <= fileScore)
			{
				indexToInsert = currentIndex;
			}
			currentIndex += 1;
		}

		ifile.close();
		//To be worked on
		insertPlayer.playTest.playerName = player->playTest.playerName;
		insertPlayer.playTest.playerScore = playerPoints;

		auto tempPlayer = fileInputPlayer.begin();
		advance(tempPlayer, indexToInsert);
		fileInputPlayer.insert(tempPlayer, insertPlayer);

		std::ofstream ofs("scoreboard.txt", std::ofstream::trunc);
		for (auto it = fileInputPlayer.begin(); it != fileInputPlayer.end(); it++)
		{
			ofs << it->playTest.playerName << "," << it->playTest.playerScore << "\n";
		}
		ofs.close();
	}
	else
	{
		//Need to be worked on
		std::cout << "file doesn't exist";
	}

	const char *message = str.c_str();
	const SDL_MessageBoxButtonData buttons[] = {
		{/* .flags, .buttonid, .text */
		 0, 0, "Exit"},
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Restart"},
	};
	const SDL_MessageBoxColorScheme colorScheme = {
		{/* .colors (.r, .g, .b) */
		 /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
		 {255, 0, 0},
		 /* [SDL_MESSAGEBOX_COLOR_TEXT] */
		 {0, 255, 0},
		 /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
		 {255, 255, 0},
		 /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
		 {0, 0, 255},
		 /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
		 {255, 0, 255}}};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,	 /* .flags */
		NULL,						 /* .window */
		"Game Over - Choose Option", /* .title */
		message,					 /* .message */
		SDL_arraysize(buttons),		 /* .numbuttons */
		buttons,					 /* .buttons */
		&colorScheme				 /* .colorScheme */
	};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
	{
		SDL_Log("error displaying message box");
		return 1;
	}
	if (buttonid == -1)
	{
		SDL_Log("no selection");
	}
	else if (buttonid == 0)
	{
		return 0;
	}
	else if (buttonid == 1)
	{
		return 1;
	}
	return 0;
}