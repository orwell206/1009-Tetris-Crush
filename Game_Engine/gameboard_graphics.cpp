#pragma once
#include <SDL2/SDL_ttf.h>
#include "colours.cpp"
#include "../Tetromino_Logic/tetromino_generator.cpp"
#include <iostream>
#define WIDTH 10
#define HEIGHT 19
#define PLAYABLE_HEIGHT 16
#define GRID_SIZE 30
#define TEXT_HORZ_ALIGN (WIDTH * GRID_SIZE + 30)
#define TEXT_VERT_ALIGN HEIGHT
SDL_Texture *font_texture = NULL;

//Image dimensions
int mWidth = 0;
int mHeight = 0;

// The phases of the game 
enum Game_Phase
{
	GAME_PHASE_PLAY,
	GAME_PHASE_LINE,
	GAME_PHASE_GAMEOVER,
};

// Text alignmnet positions 
enum Text_Align
{
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT
};

class GameBoardGraphics
{
protected:
	// Gameboard graphics variables 
	TetrominoPieceState tetrominoPiece;
	TetrominoGenerator tetrominoStruct;
	u8 next_tetromino_index = (u8)random_tetromino_index(0, tetrominoStruct.get_TetrominoShapeCount());
	Tetromino *next_tetromino;
	TetrominoPieceState next_tetrominoPiece;
	Tetromino *current_tetromino = tetrominoStruct.get_TetrominoList() + next_tetromino_index;

private:
	const Color BASE_COLORS[8] = {
		Color(0x28, 0x28, 0x28, 0xFF),	// Background/line color
		Color(0x2D, 0x99, 0x99, 0xFF),	// I shape
		Color(0x99, 0x99, 0x2D, 0xFF),	// T Shape
		Color(0x99, 0x2D, 0x99, 0xFF),	// L Shape
		Color(0x2D, 0x99, 0x51, 0xFF),	// J Shape
		Color(0x99, 0x2D, 0x2D, 0xFF),	// S Shape
		Color(0x2D, 0x63, 0x99, 0xFF),	// Z Shape
		Color(0x99, 0x63, 0x2D, 0xFF)}; // Square Shape

	const Color LIGHT_COLORS[8] = {
		Color(0x28, 0x28, 0x28, 0xFF),	// Background/line color
		Color(0x44, 0xE5, 0xE5, 0xFF),	// I Shape
		Color(0xE5, 0xE5, 0x44, 0xFF),	// T Shape
		Color(0xE5, 0x44, 0xE5, 0xFF),	// L Shape
		Color(0x44, 0xE5, 0x7A, 0xFF),	// J Shape
		Color(0xE5, 0x44, 0x44, 0xFF),	// S Shape
		Color(0x44, 0x95, 0xE5, 0xFF),	// Z Shape
		Color(0xE5, 0x95, 0x44, 0xFF)}; // Square Shape

public:
	bool loadFromRenderedText(SDL_Renderer *, TTF_Font *, const char *, SDL_Color);				// Loads text to be renderde onto the gameboard. 
	void renderText(SDL_Renderer *, s32, s32);													// Renders text onto the gameboard. 
	inline u8 get_matrix(const u8 *, s32, s32, s32);											// Gets the matrix of a tetromino
	inline void set_matrix(u8 *, s32, s32, s32, u8);											// Sets the matrix of a tetromino
	inline u8 get_tetromino(const Tetromino *, s32, s32, s32);									// Gets a tetromino object as well as its coordinates. 
	int random_tetromino_index(s32, s32); 											  			// Generates a random index which is inturn used as an index to get a random tetromino.
	void draw_text(SDL_Renderer *, TTF_Font *, const char *, s32, s32, Text_Align, Color);		// Draws text onto the gameboard. 
	void draw_on_board(SDL_Renderer *, const u8 *, s32, s32, s32, s32);				  			// Draws onto the gameboard. 
	void draw_tetromino(SDL_Renderer *, const TetrominoPieceState *, s32, s32, bool); 			// Draws the tetromino onto the gameboard.
	void draw_cell_board(SDL_Renderer *, s32, s32, u8, s32, s32, bool);				  			// Draws the individual cells of the gameboard.
	void draw_cell_preview(SDL_Renderer *, s32, s32, u8, s32, s32);								// Draws the next tetrmino preview
	void fill_rect(SDL_Renderer *, s32, s32, s32, s32, Color);					 				// Fills up a tetrimino cells with color
	void draw_rect(SDL_Renderer *, s32, s32, s32, s32, Color);						  			// Draws outline(shape) of a tetromino.
};

bool GameBoardGraphics::loadFromRenderedText(SDL_Renderer *gRenderer, TTF_Font *gFont, const char *textureText, SDL_Color textColor)
{
	/*
		Desc: Loads text to be renderde onto the gameboard. 
		
		Params: SDL_Renderer (*gRenderer), TTF_Font (*gFont), const char (*textureText), SDL_Color (textColor)
	*/
	//Render text surface
	SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);

	//Check if text surface is empty
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Empty: %s\n", TTF_GetError());
		return false;
	}

	//Create texture from surface pixels
	font_texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

	//Check if font surface is empty
	if (font_texture == NULL)
	{
		printf("Unable to create texture from rendered text! SDL Empty: %s\n", SDL_GetError());
		return false;
	}

	//Get image dimensions
	mWidth = textSurface->w;
	mHeight = textSurface->h;

	//Get rid of old surface
	SDL_FreeSurface(textSurface);

	//Return success
	return true;
}

void GameBoardGraphics::renderText(SDL_Renderer *gRenderer, s32 x, s32 y)
{
	/*
		Desc: Renders text onto the gameboard. 
		
		Params: SDL_Renderer (*gRenderer), s32 (x), s32 (y)
	*/
	//Set default params
	SDL_Rect *clip = NULL;
	double angle = 0.0;
	SDL_Point *center = NULL;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, font_texture, clip, &renderQuad, angle, center, flip);
}

inline u8 GameBoardGraphics::get_matrix(const u8 *values, s32 width, s32 row, s32 col)
{
	/*
		Desc: Gets the matrix of a tetromino.
		
		Params: const u8 (*values), s32 (width), s32 (row), s32 (col)
	*/
	s32 index = row * width + col;
	return values[index];
}

inline void GameBoardGraphics::set_matrix(u8 *values, s32 width, s32 row, s32 col, u8 value)
{
	/*
		Desc: Sets the matrix of a tetromino.
		
		Params: u8 (*values), s32 (width), s32 (row), s32 (col), u8 (value)
	*/
	s32 index = (row * width + col);
	values[index] = value;
}

inline u8 GameBoardGraphics::get_tetromino(const Tetromino *tetromino, s32 row, s32 col, s32 rotation)
{
	/*
		Desc: Gets a tetromino object as well as its coordinates. 
		
		Params: u8 const Tetromino (*tetromino), s32 (row), s32 (col), s32 (rotation)
	*/
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

int GameBoardGraphics::random_tetromino_index(s32 min, s32 max)
{
	/*
		Desc: Generates a random index which is inturn used as an index to get a random tetromino. 
		
		Params: s32 (min), s32 (max)
	*/
	s32 range = max - min;
	return min + rand() % range;
}

void GameBoardGraphics::draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, s32 x, s32 y, Text_Align alignment, Color color)
{
	/*
		Desc: Draws text onto the gameboard. 
		
		Params: SDL_Renderer (*renderer), TTF_Font (*font), const char (*text), s32 (x), s32 (y), Text_Align (alignment), Color (color)
	*/
	SDL_Color sdl_color = {color.get_r(), color.get_g(), color.get_b(), color.get_a()};
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

void GameBoardGraphics::draw_on_board(SDL_Renderer *renderer, const u8 *gameboard, s32 width, s32 height, s32 offset_x, s32 offset_y)
{
	/*
		Desc: Draws onto the gameboard. 
		
		Params: SDL_Renderer (*renderer), const u8 (*gameboard), s32 (width), s32 (height), s32 (offset_x), s32 (offset_y)
	*/
	for (s32 row = 0; row < height; row++)
	{
		for (s32 col = 0; col < width; col++)
		{
			u8 value = get_matrix(gameboard, width, row, col);
			draw_cell_board(renderer, row, col, value, offset_x, offset_y, false);
		}
	}
}

void GameBoardGraphics::draw_tetromino(SDL_Renderer *renderer, const TetrominoPieceState *tetrominoPiece, s32 offset_x, s32 offset_y, bool outline = false)
{
	/*
		Desc: Draws the tetromino onto the gameboard.
		
		Params: SDL_Renderer (*renderer), const TetrominoPieceState (*tetrominoPiece), s32 (offset_x), s32 (offset_y), bool (outline = false)
	*/
	const Tetromino *current_tetro = current_tetromino;
	const Tetromino *next_tetro = next_tetromino;

	// For gameboard tetros
	for (s32 row = 0; row < current_tetro->side; row++)
	{
		for (s32 col = 0; col < current_tetro->side; col++)
		{
			u8 value = get_tetromino(current_tetro, row, col, tetrominoPiece->get_rotation());
			if (value)
			{
				draw_cell_board(renderer, (row + tetrominoPiece->get_offset_row()), (col + tetrominoPiece->get_offset_col()), value, offset_x, offset_y, outline);
			}
		}
	}

	// For preview tetros
	for (s32 row = 0; row < next_tetro->side; row++)
	{
		for (s32 col = 0; col < next_tetro->side; col++)
		{
			u8 value2 = get_tetromino(next_tetromino, row, col, next_tetrominoPiece.get_rotation());
			if (value2)
			{
				s32 x = TEXT_HORZ_ALIGN;	   // Horizontal
				s32 y = TEXT_VERT_ALIGN + 260; // Vertical
				draw_cell_preview(renderer, row, col, value2, x, y);
			}
		}
	}
}

void GameBoardGraphics::draw_cell_board(SDL_Renderer *renderer, s32 row, s32 col, u8 value, s32 offset_x, s32 offset_y, bool outline = false)
{
	/*
		Desc: Draws the individual cells of the gameboard.
		
		Params: SDL_Renderer (*renderer), s32 (row), s32 (col), u8 (value), s32 (offset_x), s32 (offset_y), bool (outline = false)
	*/

	Color base_color = BASE_COLORS[value];
	Color light_color = LIGHT_COLORS[value];

	s32 edge = GRID_SIZE / 8;
	s32 x = col * GRID_SIZE + offset_x;
	s32 y = row * GRID_SIZE + offset_y;

	if (outline)
	{
		draw_rect(renderer, x, y, GRID_SIZE, GRID_SIZE, base_color);
		return;
	}
	fill_rect(renderer, x + edge, y, GRID_SIZE - edge, GRID_SIZE - edge, light_color);
	fill_rect(renderer, x + edge, y + edge, GRID_SIZE - edge * 2, GRID_SIZE - edge * 2, base_color);
}

void GameBoardGraphics::draw_cell_preview(SDL_Renderer *renderer, s32 row, s32 col, u8 value, s32 offset_x, s32 offset_y)
{
	/*
		Desc: Draws the next tetrmino preview. 
		
		Params: SDL_Renderer (*renderer), s32 (row), s32 (col), u8 (value), s32 (offset_x), s32 (offset_y)
	*/
	Color base_color = BASE_COLORS[value];
	Color light_color = LIGHT_COLORS[value];

	// Helps draw black "dividers"
	s32 edge = GRID_SIZE / 8;			

	// Horizontal position on board
	s32 x = col * GRID_SIZE + offset_x; 

	// Vertical position on board
	s32 y = row * GRID_SIZE + offset_y; 

	fill_rect(renderer, x + edge, y, GRID_SIZE - edge, GRID_SIZE - edge, light_color);
	fill_rect(renderer, x + edge, y + edge, GRID_SIZE - edge * 2, GRID_SIZE - edge * 2, base_color);
}

void GameBoardGraphics::fill_rect(SDL_Renderer *renderer, s32 x, s32 y, s32 width, s32 height, Color color)
{
	/*
		Desc: Fills up a tetrimino cells with color. 
		
		Params: SDL_Renderer (*renderer), s32 (x), s32 (y), s32 (width), s32 (height), Color (color)
	*/

	SDL_Rect rect = {};
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_SetRenderDrawColor(renderer, color.get_r(), color.get_g(), color.get_b(), color.get_a());
	SDL_RenderFillRect(renderer, &rect);
}

void GameBoardGraphics::draw_rect(SDL_Renderer *renderer, s32 x, s32 y, s32 width, s32 height, Color color)
{
	/*
		Desc:  Draws outline(shape) of a tetromino.
		
		Params: SDL_Renderer (*renderer), s32 (x), s32 (y), s32 (width), s32 (height), Color (color)
	*/
	SDL_Rect rect = {};
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_SetRenderDrawColor(renderer, color.get_r(), color.get_g(), color.get_b(), color.get_a());
	SDL_RenderDrawRect(renderer, &rect);
}