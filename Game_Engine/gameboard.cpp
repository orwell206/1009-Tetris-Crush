#include "../common_data.hpp"
#include "tetromino_piece_state.cpp"
#include "tetromino.cpp"
#include "colours.hpp"
#include "../Player_Engine/input_state.cpp"
#include "../Player_Engine/player.cpp"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#define WIDTH 10
#define HEIGHT 19
#define PLAYABLE_HEIGHT 16
#define GRID_SIZE 30

const u8 FRAMES_PER_DROP[] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};
const f32 TARGET_SECONDS_PER_FRAME = 1.f / 60.f;

enum Game_Phase
{
	GAME_PHASE_PLAY,
	GAME_PHASE_LINE,
	GAME_PHASE_GAMEOVER,
};
enum Text_Align
{
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT
};

class GameBoard
{
private:
	u8 gameboard[WIDTH * HEIGHT];
	u8 lines[HEIGHT];
	s32 pending_line_count;

	TetrominoPieceState tetrominoPiece;
	TetrominoGenerator tetrominoStruct;

	u8 next_tetromino_index = (u8)random_tetromino_index(0, tetrominoStruct.get_TetrominoShapeCount());
	Tetromino *next_tetromino;
	Tetromino *current_tetromino;

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
	void draw_text(SDL_Renderer *, TTF_Font *, const char *, s32, s32, Text_Align, Color);
	void render_clearline_graphics(const GameBoard *, SDL_Renderer *);
	void render_game(const GameBoard *, SDL_Renderer *, TTF_Font *);
	void draw_on_board(SDL_Renderer *, const u8 *, s32, s32, s32, s32);
	void draw_tetromino(SDL_Renderer *, const TetrominoPieceState *, s32, s32, bool);
	void draw_cell(SDL_Renderer *, s32, s32, u8, s32, s32, bool);
	void fill_rect(SDL_Renderer *, s32, s32, s32, s32, Color);
	void draw_rect(SDL_Renderer *, s32, s32, s32, s32, Color);
	inline u8 get_tetromino(const Tetromino *, s32, s32, s32);
	inline u8 check_if_row_filled(const u8 *, s32, s32);
	inline u8 check_if_row_empty(const u8 *, s32, s32);
	s32 find_lines(const u8 *, s32, s32, u8 *);
	void clear_lines(u8 *, s32, s32, const u8 *);
	inline u8 get_matrix(const u8 *, s32, s32, s32);
	inline void set_matrix(u8 *, s32, s32, s32, u8);
	int update_game(GameBoard *, const InputState *, SDL_Renderer *, TTF_Font *);
	bool check_tetromino_valid(const TetrominoPieceState *, const u8 *, s32, s32);
	void spawn_tetromino(GameBoard *);
	int random_tetromino_index(s32, s32);
	void merge_tetrimino_on_board(GameBoard *, Player *);
	inline bool drop_tetromino(GameBoard *, Player *);
	inline f32 get_time_to_next_tetromino_drop(s32);
	void update_gameline(GameBoard *);
	void update_gameplay(GameBoard *, const InputState *);
	int game_Over(GameBoard *, SDL_Renderer *, TTF_Font *);
};

f32 GameBoard::get_GameBoardTime() { return time; }

void GameBoard::set_GameBoardTime(f32 time) { this->time = time; }

void GameBoard::set_player(Player &player) { this->player = &player; }

void GameBoard::draw_text(SDL_Renderer *renderer,
						  TTF_Font *font,
						  const char *text,
						  s32 x, s32 y,
						  Text_Align alignment,
						  Color color)
{
	SDL_Color sdl_color = {color.r, color.g, color.b, color.a};
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, sdl_color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect rect;
	rect.y = y;
	rect.w = surface->w;
	rect.h = surface->h;
	switch (alignment)
	{
	case TEXT_ALIGN_LEFT:
		rect.x = x;
		break;
	case TEXT_ALIGN_CENTER:
		rect.x = x - surface->w / 2;
		break;
	case TEXT_ALIGN_RIGHT:
		rect.x = x - surface->w;
		break;
	}

	SDL_RenderCopy(renderer, texture, 0, &rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
void GameBoard::render_clearline_graphics(const GameBoard *gameboard, SDL_Renderer *renderer)
{
	Color highlight_color = color(0xFF, 0xFF, 0xFF, 0xFF);
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
			tetro_shadow.offset_row++;
		}
		--tetro_shadow.offset_row;

		draw_tetromino(renderer, &tetro_shadow, 0, 0, true);
	}

	render_clearline_graphics(gameboard, renderer);

	//temp for rendering score text on GUI
	char buffer[4096];
	s32 x = WIDTH * GRID_SIZE * 1.5;
	//s32 margin_y = 60;
	s32 y = HEIGHT;
	Color highlight_color = color(0xFF, 0xFF, 0xFF, 0xFF);
	//const char* playerScore =  *player->score
	//draw_text(renderer, font, "test", x, y, TEXT_ALIGN_CENTER, highlight_color);
	//gameboard->points = 0;
	snprintf(buffer, sizeof(buffer), "POINTS: %d", gameboard->points);
	draw_text(renderer, font, buffer, x, y, TEXT_ALIGN_LEFT, highlight_color);
}

void GameBoard::draw_on_board(SDL_Renderer *renderer, const u8 *gameboard, s32 width, s32 height, s32 offset_x, s32 offset_y)
{
	for (s32 row = 0; row < height; row++)
	{
		for (s32 col = 0; col < width; col++)
		{
			u8 value = get_matrix(gameboard, width, row, col);
			draw_cell(renderer, row, col, value, offset_x, offset_y, false);
		}
	}
}

void GameBoard::draw_tetromino(SDL_Renderer *renderer, const TetrominoPieceState *tetrominoPiece, s32 offset_x, s32 offset_y, bool outline = false)
{
	// const Tetromino *current_tetromino = tetrominoStruct.get_TetrominoList() + tetrominoPiece->tetromino_index;
	const Tetromino *current_tetromino = next_tetromino;

	for (s32 row = 0; row < current_tetromino->side; row++)
	{
		for (s32 col = 0; col < current_tetromino->side; col++)
		{
			u8 value = get_tetromino(current_tetromino, row, col, tetrominoPiece->rotation);
			if (value)
			{
				draw_cell(renderer, row + tetrominoPiece->offset_row, col + tetrominoPiece->offset_col, value, offset_x, offset_y, outline);
			}
		}
	}
}

void GameBoard::draw_cell(SDL_Renderer *renderer, s32 row, s32 col, u8 value, s32 offset_x, s32 offset_y, bool outline = false)
/*
* Main logic gameboard and tetromino colors
*/
{
	Color base_color = BASE_COLORS[value];
	Color light_color = LIGHT_COLORS[value];
	Color dark_color = DARK_COLORS[value];

	s32 edge = GRID_SIZE / 8;
	s32 x = col * GRID_SIZE + offset_x;
	s32 y = row * GRID_SIZE + offset_y;

	if (outline)
	{
		draw_rect(renderer, x, y, GRID_SIZE, GRID_SIZE, base_color);
		return;
	}
	// fill_rect(renderer, x, y, GRID_SIZE, GRID_SIZE, dark_color); // Disabled to have "grid" effect
	fill_rect(renderer, x + edge, y, GRID_SIZE - edge, GRID_SIZE - edge, light_color);
	fill_rect(renderer, x + edge, y + edge, GRID_SIZE - edge * 2, GRID_SIZE - edge * 2, base_color);
}

void GameBoard::fill_rect(SDL_Renderer *renderer, s32 x, s32 y, s32 width, s32 height, Color color)
/*
* For main gameboard and tetromino
*/
{
	SDL_Rect rect = {};
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
}

void GameBoard::draw_rect(SDL_Renderer *renderer, s32 x, s32 y, s32 width, s32 height, Color color)
/*
* For the tetromino "shadow", draw but don't fill
*/
{
	SDL_Rect rect = {};
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &rect);
}

inline u8 GameBoard::get_tetromino(const Tetromino *tetromino, s32 row, s32 col, s32 rotation)
{
	s32 side = tetromino->side;
	switch (rotation)
	{
	case 0:
		return tetromino->matrix[row * side + col];
	case 1:
		return tetromino->matrix[(side - col - 1) * side + row];
	case 2:
		return tetromino->matrix[(side - row - 1) * side + (side - col - 1)];
	case 3:
		return tetromino->matrix[col * side + (side - row - 1)];
	}
	return 0;
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

inline u8 GameBoard::get_matrix(const u8 *values, s32 width, s32 row, s32 col)
{
	s32 index = row * width + col;
	return values[index];
}

inline void GameBoard::set_matrix(u8 *values, s32 width, s32 row, s32 col, u8 value)
{
	s32 index = (row * width + col);
	values[index] = value;
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
	// const Tetromino *tetromino = tetrominoStruct.get_TetrominoList() + tetromino_piece->tetromino_index;
	const Tetromino *tetromino = next_tetromino;

	assert(tetromino);

	for (s32 row = 0; row < tetromino->side; row++)
	{
		for (s32 col = 0; col < tetromino->side; col++)
		{
			u8 value = get_tetromino(tetromino, row, col, tetromino_piece->rotation);
			if (value > 0)
			{
				s32 board_row = tetromino_piece->offset_row + row;
				s32 board_col = tetromino_piece->offset_col + col;
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
	gameboard->tetrominoPiece = {};
	gameboard->tetrominoPiece.tetromino_index = next_tetromino_index;
	gameboard->tetrominoPiece.offset_col = WIDTH / 2;
	gameboard->nextDropTime = gameboard->time + get_time_to_next_tetromino_drop(gameboard->level);

	next_tetromino_index = (u8)random_tetromino_index(0, tetrominoStruct.get_TetrominoShapeCount());
	next_tetromino = tetrominoStruct.get_TetrominoList() + next_tetromino_index;

	std::cout << "-" << std::endl;
	std::cout << next_tetromino_index << std::endl;
}

int GameBoard::random_tetromino_index(s32 min, s32 max)
{
	s32 range = max - min;
	return min + rand() % range;
}

void GameBoard::merge_tetrimino_on_board(GameBoard *gameboard, Player *player)
{
	// const Tetromino *tetromino = tetrominoStruct.get_TetrominoList() + gameboard->tetrominoPiece.tetromino_index;
	const Tetromino *tetromino = next_tetromino;

	for (s32 row = 0; row < tetromino->side; row++)
	{
		for (s32 col = 0; col < tetromino->side; col++)
		{
			u8 value = get_tetromino(tetromino, row, col, gameboard->tetrominoPiece.rotation);
			if (value)
			{
				s32 board_row = gameboard->tetrominoPiece.offset_row + row;
				s32 board_col = gameboard->tetrominoPiece.offset_col + col;
				set_matrix(gameboard->gameboard, WIDTH, board_row, board_col, value); // problem
			}
		}
	}
}

inline bool GameBoard::drop_tetromino(GameBoard *gameboard, Player *player)
{
	++gameboard->tetrominoPiece.offset_row;
	if (!check_tetromino_valid(&gameboard->tetrominoPiece, gameboard->gameboard, WIDTH, HEIGHT))
	{
		--gameboard->tetrominoPiece.offset_row;
		merge_tetrimino_on_board(gameboard, player);
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
	if (input->da > 1)
	{
		--tetrominoPiece.offset_col;
	}
	if (input->dd > 1)
	{
		++tetrominoPiece.offset_col;
	}
	if (input->dw > 1)
	{
		tetrominoPiece.rotation = (tetrominoPiece.rotation + 1) % 4;
	}

	if (check_tetromino_valid(&tetrominoPiece, gameboard->gameboard, WIDTH, HEIGHT))
	{
		gameboard->tetrominoPiece = tetrominoPiece;
	}

	if (input->ds > 1)
	{
		drop_tetromino(gameboard, player);
	}

	if (input->dspace > 1)
	{
		while (drop_tetromino(gameboard, player))
			;
	}

	while (gameboard->time >= gameboard->nextDropTime)
	{
		drop_tetromino(gameboard, player);
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
	std::string str = "Continue or Exit?\n Highest Points: " + std::to_string(playerPoints);
	const char *message = str.c_str();
	const SDL_MessageBoxButtonData buttons[] = {
		{/* .flags, .buttonid, .text */ 0, 0, "Exit"},
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