#include "../common_data.hpp"
#include "tetromino_piece_state.cpp"
#include "tetromino.cpp"
#include "../Player_Engine/input_state.cpp"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#define WIDTH 10
#define HEIGHT 19
#define PLAYABLE_HEIGHT 16
#define GRID_SIZE 30
#define FINAL_ROW 0 

const u8 FRAMES_PER_DROP[] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};
const f32 TARGET_SECONDS_PER_FRAME = 1.f / 60.f;


enum Game_Phase{
	GAME_PHASE_PLAY,
	GAME_PHASE_LINE,
	GAME_PHASE_GAMEOVER,
};
enum Text_Align{
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTER,
		TEXT_ALIGN_RIGHT
};

class GameBoard {
public:
	u8 gameboard[WIDTH * HEIGHT];
	u8 lines[HEIGHT];
	s32 pending_line_count;

	TetrominoPieceState tetrominoPiece;

	Player *player;
	Game_Phase gamePhase;
	f32 nextDropTime;
	f32 highlighted_end_time;
	f32 time;
	s32 level;
	s32 cleared_lines;
	s32 line_count;
	s32 points;


	void set_player(Player &player){
		this->player = &player;
	}

	void draw_text(SDL_Renderer *renderer,
            TTF_Font *font,
            const char *text,
            s32 x, s32 y,
            Text_Align alignment,
            Color color){
    SDL_Color sdl_color = SDL_Color { color.r, color.g, color.b, color.a };
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
	void render_clearline_graphics(const GameBoard *gameboard, SDL_Renderer *renderer){
		Color highlight_color = color(0xFF, 0xFF, 0xFF, 0xFF);
		if (gameboard->gamePhase == GAME_PHASE_LINE){
			for (s32 row = 0; row < HEIGHT; row++){
				if (gameboard->lines[row]){
					s32 x = 0;
					s32 y = row * GRID_SIZE;
					fill_rect(renderer, x, y, WIDTH * GRID_SIZE, GRID_SIZE, highlight_color);
				}
			}
		}

	}


	void render_game(const GameBoard *gameboard, SDL_Renderer *renderer,TTF_Font *font){
		draw_on_board(renderer, gameboard->gameboard, WIDTH, HEIGHT, 0, 0);
		draw_tetromino(renderer, &gameboard->tetrominoPiece, 0, 0);
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


	void draw_on_board(SDL_Renderer *renderer, const u8 *gameboard, s32 width, s32 height, s32 offset_x, s32 offset_y){
		for (s32 row = 0; row < height; row++){
			for (s32 col = 0; col < width; col++){
				u8 value = get_matrix(gameboard, width, row, col);
				draw_cell(renderer, row, col, value, offset_x, offset_y);
			}
		}
	}


	void draw_tetromino(SDL_Renderer *renderer, const TetrominoPieceState *tetrominoPiece, s32 offset_x, s32 offset_y){
		const Tetromino *tempTetromino = TETROMINOS.tetromino_list + tetrominoPiece->tetromino_index;
		for (s32 row = 0; row < tempTetromino->side; row++){
			for (s32 col = 0; col < tempTetromino->side; col++){
				u8 value = get_tetromino(tempTetromino, row, col, tetrominoPiece->rotation);
				if (value){
					draw_cell(renderer, row + tetrominoPiece->offset_row, col + tetrominoPiece->offset_col, value, offset_x, offset_y);
				}
			}
		}
	}


	void draw_cell(SDL_Renderer *renderer, s32 row, s32 col, u8 value, s32 offset_x, s32 offset_y){
		Color base_color = BASE_COLORS[value];
		Color light_color = LIGHT_COLORS[value];
		Color dark_color = DARK_COLORS[value];

		s32 edge = GRID_SIZE / 8;
		s32 x = col * GRID_SIZE + offset_x;
		s32 y = row * GRID_SIZE + offset_y;

		fill_rect(renderer, x, y, GRID_SIZE, GRID_SIZE, dark_color);
		fill_rect(renderer, x + edge, y, GRID_SIZE - edge, GRID_SIZE - edge, light_color);
		fill_rect(renderer, x + edge, y + edge, GRID_SIZE - edge * 2, GRID_SIZE -edge * 2, base_color);
	}


	inline u8 get_tetromino(const Tetromino *tetromino, s32 row, s32 col, s32 rotation){
		s32 side = tetromino->side;
		switch(rotation){
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


	inline u8 check_if_row_filled(const u8 *values, s32 width, s32 row){
		for (s32 col = 0; col < width; col++){
			if (!get_matrix(values, width, row, col)){
				return 0;
			}
		}
		return 1;
	}


	inline u8 check_if_row_empty(const u8 *values, s32 width, s32 row){
		for (s32 col = 0; col < width; col++){
			if (get_matrix(values, width, row, col)){
				return 0;
			}
		}
		return 1;
	}







	s32 find_lines(const u8 *values, s32 width, s32 height, u8 *lines_out){
		s32 count = 0;
		for (s32 row = 0; row < height; row++){
			u8 filled = check_if_row_filled(values, width, row);
			lines_out[row] = filled;
			count += filled;
		}
		return count;
	}


	void clear_lines(u8 *values, s32 width, s32 height, const u8 *lines){
		s32 src_row = height - 1;
		for (s32 dst_row = height - 1; dst_row >= 0; dst_row--){
			while (src_row > 0 && lines[src_row]){
				src_row--;
			}

			if (src_row < 0){
				memset(values + dst_row * width, 0, width);
			}
			else {
				memcpy(values + dst_row * width, values + src_row * width, width);
				src_row--;
			}
		}
	}


	inline u8 get_matrix(const u8 *values, s32 width, s32 row, s32 col){
		s32 index = row * width + col;
		return values[index];
	}


	inline void set_matrix(u8 *values, s32 width, s32 row, s32 col, u8 value){
		s32 index = (row * width + col);
		values[index] = value;
	}


	void update_game(GameBoard *gameboard, const InputState *input){
		switch(gameboard->gamePhase){
			case GAME_PHASE_PLAY:
				update_gameplay(gameboard, input);
				break;

			case GAME_PHASE_LINE:
				update_gameline(gameboard);
				break;
				
			case GAME_PHASE_GAMEOVER:
				exit(1);
		}
	}


	void fill_rect(SDL_Renderer *renderer, s32 x, s32 y, s32 width, s32 height, Color color){
		SDL_Rect rect = {};
		rect.x = x;
		rect.y = y;
		rect.w = width;
		rect.h = height;
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);
	}


	bool check_tetromino_valid(const TetrominoPieceState *tetromino_piece, const u8 *gameboard, s32 width, s32 height){
		const Tetromino *tetromino = TETROMINOS.tetromino_list + tetromino_piece->tetromino_index;
		assert(tetromino);

		for (s32 row = 0; row < tetromino->side; row++){
			for (s32 col = 0; col < tetromino->side; col++){
				u8 value = get_tetromino(tetromino, row, col, tetromino_piece->rotation);
				if (value > 0){
					s32 board_row = tetromino_piece->offset_row + row;
					s32 board_col = tetromino_piece->offset_col + col;
					if (board_row < 0){
						return false;
					}
					if (board_row >= height){
						return false;
					}
					if (board_col < 0){
						return false;
					}
					if (board_col >= width){
						return false;
					}
					if (get_matrix(gameboard, width, board_row, board_col)){
						return false;
					}
				}
			}
		}
		return true;
	}


	void spawn_tetromino(GameBoard *gameboard){
		gameboard->tetrominoPiece = {};
		gameboard->tetrominoPiece.tetromino_index = (u8)random_tetromino_index(0, TETROMINOS.tetromino_shape_count);
		gameboard->tetrominoPiece.offset_col = WIDTH / 2;
	}


	int random_tetromino_index(s32 min, s32 max){
		s32 range = max - min;
		return min + rand() % range; 

	}

	void merge_tetrimino_on_board(GameBoard *gameboard, Player *player){
		const Tetromino *tetromino = TETROMINOS.tetromino_list + gameboard->tetrominoPiece.tetromino_index;
		for (s32 row = 0; row < tetromino->side; row++){
			for (s32 col = 0; col < tetromino->side; col++){
				u8 value = get_tetromino(tetromino, row, col, gameboard->tetrominoPiece.rotation);
				if (value){
					s32 board_row = gameboard->tetrominoPiece.offset_row + row;
					s32 board_col = gameboard->tetrominoPiece.offset_col + col;
					set_matrix(gameboard->gameboard, WIDTH, board_row, board_col, value); // problem
				}
			}
		}
	}


	inline bool drop_tetromino(GameBoard *gameboard, Player *player){
		++gameboard->tetrominoPiece.offset_row;
		if (!check_tetromino_valid(&gameboard->tetrominoPiece, gameboard->gameboard, WIDTH, HEIGHT)){
			--gameboard->tetrominoPiece.offset_row;
			merge_tetrimino_on_board(gameboard, player);
			spawn_tetromino(gameboard);
			return false;
		}
		gameboard->nextDropTime = gameboard->time + get_time_to_next_tetromino_drop(gameboard->level);
		return true;
	}



	inline f32 get_time_to_next_tetromino_drop(s32 level){
		if (level > 29){
			level = 29;
		}
		return FRAMES_PER_DROP[level] * TARGET_SECONDS_PER_FRAME;
	}


	void update_gameline(GameBoard *gameboard){
		if (gameboard->time >= gameboard->highlighted_end_time){
			clear_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
			gameboard->line_count += gameboard->pending_line_count;
			//For debugging
			std::cout << gameboard->pending_line_count;
			//Temp adding of score
			gameboard->points += 20 * gameboard->pending_line_count;
			gameboard->gamePhase = GAME_PHASE_PLAY;
			
		}
	}


	void update_gameplay(GameBoard *gameboard, const InputState *input){

		TetrominoPieceState tetrominoPiece = gameboard->tetrominoPiece;
		if (input->da > 1){
			--tetrominoPiece.offset_col;
		}
		if (input->dd > 1){
			++tetrominoPiece.offset_col;
		}
		if (input->dw > 1){
			tetrominoPiece.rotation = (tetrominoPiece.rotation + 1) % 4;
		}

		if (check_tetromino_valid(&tetrominoPiece, gameboard->gameboard, WIDTH, HEIGHT)){
			gameboard->tetrominoPiece = tetrominoPiece;
		}

		if (input->ds > 1){
			drop_tetromino(gameboard, player);
		}

		if (input->dspace > 1){
			while (drop_tetromino(gameboard, player));
		}


		while (gameboard->time >= gameboard->nextDropTime){
			drop_tetromino(gameboard, player);
		}

		s32 line_count = find_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
		//temp for adding pending line count 
		gameboard->pending_line_count = find_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
		if (line_count > 0){
			gameboard->gamePhase = GAME_PHASE_LINE;
			gameboard->highlighted_end_time = gameboard->time + 0.5f;
		}

		if (!check_if_row_empty(gameboard->gameboard, WIDTH, FINAL_ROW)){
			gameboard->gamePhase = GAME_PHASE_GAMEOVER;
		}
	}
};
