#pragma once
#include "../common_data.hpp"
#include "../Tetromino_Logic/tetromino_piece_state.cpp"
#include "../Tetromino_Logic/tetromino.cpp"
#include "gameboard_graphics.cpp"
#include "../Player_Engine/input_state.cpp"
#include "../Player_Engine/player.cpp"
#include "../Audio_Engine/sound_effect.cpp"
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
	// Gameboard variables
	u8 gameboard[WIDTH * HEIGHT]; // Gameboard object
	u8 lines[HEIGHT];			  // Lines of the gameboard
	s32 pending_line_count;		  // Pending line count
	Player *player;				  // Player object
	Game_Phase gamePhase;		  // Game phase
	f32 nextDropTime;			  // Time till the next tetromino drops
	f32 highlighted_end_time;	  // Highlighted end time
	f32 time;					  // Game time
	s32 level;					  // Game level
	s32 cleared_lines;			  // Number of lines successfully cleared
	s32 line_count;				  // How many lines have been filled in the gameboard
	s32 points;					  // Player score

public:
	f32 get_GameBoardTime();													   // Gets the gameboard time.
	void set_GameBoardTime(f32);												   // Sets the gameboard time.
	void set_player(Player &);													   // Sets the player object.
	void render_clearline_graphics(const GameBoard *, SDL_Renderer *);			   // Renders the clearing of line graphics.
	void render_game(const GameBoard *, SDL_Renderer *, TTF_Font *);			   // Renders the gameboard by drawing the gameboard cells and tetrminos.
	inline u8 check_if_row_filled(const u8 *, s32, s32);						   // Checks if a row on the gameboard is filled with tetrominos.
	inline u8 check_if_row_empty(const u8 *, s32, s32);							   // Checks if a row on the  gameboard is empty.
	s32 find_lines(const u8 *, s32, s32, u8 *);									   // Find rows that are filled with tetrominos.
	void clear_lines(u8 *, s32, s32, const u8 *);								   // Clears a line on the gameboard if it is filled with tetrominos
	int update_game(GameBoard *, const InputState *, SDL_Renderer *, TTF_Font *);  // Updates the game based on the state of the game.
	bool check_tetromino_valid(const TetrominoPieceState *, const u8 *, s32, s32); // Checks whether a tetromino's position on the gameboard is valid.
	void spawn_tetromino(GameBoard *);											   // Spawns a tetrmino onto the gameboard as well as for the preview.
	void merge_tetrimino_on_board(GameBoard *);									   // Merges the tetrimino onto the gameboard's cells.
	inline bool drop_tetromino(GameBoard *);									   // Drops a tetromino onto the gameboard.
	inline f32 get_time_to_next_tetromino_drop(s32);							   // Gets the time to drop the next tetromino onto the gameboard.
	void update_gameline(GameBoard *);											   // Updates the gamelines; clear the gameline if it is filled with tetrminos.
	void update_gameplay(GameBoard *, const InputState *);						   // Updates the moving of the tetriminos based on the player's controls.
	int game_Over(GameBoard *, SDL_Renderer *, TTF_Font *);						   // Displays the highscores of the game as well as prompt the user to restart or exit the game.
};

f32 GameBoard::get_GameBoardTime()
{
	/*
		Desc: Gets the gameboard time.

		Params: None.
	*/
	return time;
}

void GameBoard::set_GameBoardTime(f32 time)
{
	/*
		Desc: Sets the gameboard time.

		Params: f32 (time)
	*/
	this->time = time;
}

void GameBoard::set_player(Player &player)
{
	/*
		Desc: Sets the player object.

		Params: Player (&player)
	*/
	this->player = &player;
}

void GameBoard::render_clearline_graphics(const GameBoard *gameboard, SDL_Renderer *renderer)
{
	/*
		Desc: Renders the clearing of line graphics.

		Params: const GameBoard (*gameboard), SDL_Renderer (*renderer)
	*/
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
	/*
		Desc: Renders the gameboard by drawing the gameboard cells and tetrminos.

		Params: const GameBoard (*gameboard), SDL_Renderer (*renderer), TTF_Font (*font)
	*/
	draw_on_board(renderer, gameboard->gameboard, WIDTH, HEIGHT, 0, 0);

	// Check that the gamephase is game_phase_play
	if (gameboard->gamePhase == GAME_PHASE_PLAY)
	{
		draw_tetromino(renderer, &gameboard->tetrominoPiece, 0, 0, false);
		TetrominoPieceState tetro_shadow = gameboard->tetrominoPiece;
		while (check_tetromino_valid(&tetro_shadow, gameboard->gameboard, WIDTH, HEIGHT))
		{
			s32 temp = tetro_shadow.get_offset_row();
			tetro_shadow.set_offset_row(temp + 1);
		}
		s32 temp = tetro_shadow.get_offset_row();
		tetro_shadow.set_offset_row(temp - 1);
		draw_tetromino(renderer, &tetro_shadow, 0, 0, true);
	}

	// Render the clearline graphics
	render_clearline_graphics(gameboard, renderer);

	// For rendering score text on GUI
	char buffer[4096];
	s32 x = TEXT_HORZ_ALIGN;
	s32 y = TEXT_VERT_ALIGN;
	Color highlight_color = Color(0xFF, 0xFF, 0xFF, 0xFF);
	draw_text(renderer, font, "Player Name:", x, y, TEXT_ALIGN_LEFT, highlight_color);
	draw_text(renderer, font, player->playerInfo.playerName.c_str(), x, y + 50, TEXT_ALIGN_LEFT, highlight_color);
	draw_text(renderer, font, "POINTS:", x, y + 100, TEXT_ALIGN_LEFT, highlight_color);
	snprintf(buffer, sizeof(buffer), "%d", player->playerInfo.playerScore);
	draw_text(renderer, font, buffer, x, (y + 150), TEXT_ALIGN_LEFT, highlight_color);
	draw_text(renderer, font, "NEXT BLOCK:", x, (y + 210), TEXT_ALIGN_LEFT, highlight_color);
}

inline u8 GameBoard::check_if_row_filled(const u8 *values, s32 width, s32 row)
{
	/*
		Desc: Checks if a row on the gameboard is filled with tetrominos.

		Params: const u8 (*values), s32 (width), s32 (row)
	*/
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
	/*
		Desc: Checks if a row on the  gameboard is empty.

		Params: const u8 (*values), s32 (width), s32 (row)
	*/
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
	/*
		Desc: Find rows that are filled with tetrominos.

		Params: const u8 (*values), s32 (width), s32 (height), u8 (*lines_out)
	*/
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
	/*
		Desc: Clears a line on the gameboard if it is filled with tetrominos.

		Params: u8 (*values), s32 (width), s32 (height), const u8 (*lines)
	*/
	// Play sound effect
	Mix_Chunk *clearSound = Mix_LoadWAV(PATH_SE_CLEAR);
	SoundEffect SEclearSound;
	SEclearSound.load(clearSound);
	SEclearSound.play();

	// Clear the lines
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
	/*
		Desc: Updates the game based on the state of the game.

		Params: GameBoard (*gameboard), const InputState (*input), SDL_Renderer (*renderer), TTF_Font (*font)
	*/
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
	/*
		Desc: Checks whether a tetromino's position on the gameboard is valid.

		Params: const TetrominoPieceState (*tetromino_piece), const u8 (*gameboard), s32 (width), s32 (height)
	*/
	const Tetromino *tetromino = current_tetromino;
	assert(tetromino);

	// Perform the checks on whether the tetromino piece can be rotated or moved.
	for (s32 row = 0; row < tetromino->side; row++)
	{
		for (s32 col = 0; col < tetromino->side; col++)
		{
			u8 value = get_tetromino(tetromino, row, col, tetromino_piece->get_rotation());
			if (value > 0)
			{
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
	/*
		Desc: Spawns a tetrmino onto the gameboard as well as for the preview.

		Params: GameBoard (*gameboard)
	*/

	// Grabs "next tetromino" and preps it for spawn
	gameboard->tetrominoPiece.resetState();
	gameboard->tetrominoPiece.set_tetromino_index(next_tetromino_index);
	gameboard->tetrominoPiece.set_offset_col(WIDTH / 2);
	gameboard->nextDropTime = gameboard->time + get_time_to_next_tetromino_drop(gameboard->level);

	// Set "current tetromino" to the one prep'ed above
	current_tetromino = tetrominoStruct.get_TetrominoList() + gameboard->tetrominoPiece.get_tetromino_index();

	// Select new random "next tetromino"
	next_tetromino_index = (u8)random_tetromino_index(0, tetrominoStruct.get_TetrominoShapeCount());
	next_tetromino = tetrominoStruct.get_TetrominoList() + next_tetromino_index;
}

void GameBoard::merge_tetrimino_on_board(GameBoard *gameboard)
{
	/*
		Desc: Merges the tetrimino onto the gameboard's cells.

		Params: GameBoard (*gameboard)
	*/

	// Play sound effect
	Mix_Chunk *fallTetris = Mix_LoadWAV(PATH_SE_FALL);
	SoundEffect SEfallTetris;
	SEfallTetris.load(fallTetris);
	SEfallTetris.play();

	const Tetromino *tetromino = current_tetromino;

	// Merge the current tetrmino onto the gameboard's cells
	for (s32 row = 0; row < tetromino->side; row++)
	{
		for (s32 col = 0; col < tetromino->side; col++)
		{
			u8 value = get_tetromino(tetromino, row, col, gameboard->tetrominoPiece.get_rotation());
			if (value)
			{
				s32 board_row = gameboard->tetrominoPiece.get_offset_row() + row;
				s32 board_col = gameboard->tetrominoPiece.get_offset_col() + col;
				set_matrix(gameboard->gameboard, WIDTH, board_row, board_col, value);
			}
		}
	}
}

inline bool GameBoard::drop_tetromino(GameBoard *gameboard)
{
	/*
		Desc: Drops a tetromino onto the gameboard.

		Params: GameBoard (*gameboard)
	*/
	s32 temp = gameboard->tetrominoPiece.get_offset_row();
	gameboard->tetrominoPiece.set_offset_row(temp + 1);
	if (!check_tetromino_valid(&gameboard->tetrominoPiece, gameboard->gameboard, WIDTH, HEIGHT))
	{
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
	/*
		Desc: Gets the time to drop the next tetromino onto the gameboard.

		Params: s32 (level)
	*/
	if (level > 29)
	{
		level = 29;
	}
	return FRAMES_PER_DROP[level] * TARGET_SECONDS_PER_FRAME;
}

void GameBoard::update_gameline(GameBoard *gameboard)
{
	/*
		Desc: Updates the gamelines; clear the gameline if it is filled with tetrminos.

		Params: GameBoard (*gameboard)
	*/
	if (gameboard->time >= gameboard->highlighted_end_time)
	{
		// Clears the gameline
		clear_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
		gameboard->line_count += gameboard->pending_line_count;
		int pendingLineCount = gameboard->pending_line_count;

		// Adding of score
		player->playerInfo.playerScore = player->awardPoints(player, pendingLineCount);
		gameboard->gamePhase = GAME_PHASE_PLAY;
	}
}

void GameBoard::update_gameplay(GameBoard *gameboard, const InputState *input)
{
	/*
		Desc: Updates the moving of the tetriminos based on the player's controls.

		Params: GameBoard (*gameboard), const InputState (*input)
	*/
	TetrominoPieceState tetrominoPiece = gameboard->tetrominoPiece;
	// Moves the tetromino left
	if (input->get_da() > 1)
	{
		s32 temp = tetrominoPiece.get_offset_col();
		tetrominoPiece.set_offset_col(temp - 1);
	}

	// Moves the tetromino right
	if (input->get_dd() > 1)
	{
		s32 temp = tetrominoPiece.get_offset_col();
		tetrominoPiece.set_offset_col(temp + 1);
	}

	// Rotate the tetromino
	if (input->get_dw() > 1)
	{
		s32 temp = tetrominoPiece.get_rotation();
		tetrominoPiece.set_rotation((temp + 1) % 4);
	}

	if (check_tetromino_valid(&tetrominoPiece, gameboard->gameboard, WIDTH, HEIGHT))
	{
		gameboard->tetrominoPiece = tetrominoPiece;
	}

	// Moves the tetromino down
	if (input->get_ds() > 1)
	{
		drop_tetromino(gameboard);
	}

	// Drops the tetromino
	if (input->get_dspace() > 1)
	{
		while (drop_tetromino(gameboard))
			;
	}

	while (gameboard->time >= gameboard->nextDropTime)
	{
		drop_tetromino(gameboard);
	}

	// Search for lines that are filled fully with tetrominos
	s32 line_count = find_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
	gameboard->pending_line_count = find_lines(gameboard->gameboard, WIDTH, HEIGHT, gameboard->lines);
	if (line_count > 0)
	{
		gameboard->gamePhase = GAME_PHASE_LINE;
		gameboard->highlighted_end_time = gameboard->time + 0.5f;
	}

	// Checks whether the game is over
	s32 game_over_row = 0;
	if (!check_if_row_empty(gameboard->gameboard, WIDTH, game_over_row))
	{
		gameboard->gamePhase = GAME_PHASE_GAMEOVER;
	}
}

int GameBoard::game_Over(GameBoard *gameboard, SDL_Renderer *renderer, TTF_Font *font)
{
	/*
		Desc: Displays the highscores of the game as well as prompt the user to restart or exit the game.

		Params: GameBoard (*gameboard), SDL_Renderer (*renderer), TTF_Font (*font)
	*/
	//Retrieve last known player score
	int playerPoints = player->playerInfo.playerScore;
	//Init index for usage
	int indexToInsert, currentIndex = 0;
	//Message to be displayed out on the popout
	std::string str = "Continue or Exit?\nHighest Points: " + std::to_string(playerPoints) + "\nLeaderBoard: " + "\nName:Score";

	/* try to open file to read */
	try
	{
		//Open file
		std::ifstream ifile;
		ifile.open("scoreboard.txt");
		//Check if file exist.
		if (!ifile)
		{
			//Doesnt contain file
			std::cout << "file doesn't exist";
			//Create new file
			FILE *file;
			file = fopen("scoreboard.txt", "r+b");
			ifile.open("scoreboard.txt");
			fclose(file);
		}
		//Declare variables for usage
		std::string line;
		std::list<Player> fileInputPlayer;
		Player insertPlayer;
		//Loop to retrieve all lines from the file.
		while (getline(ifile, line, ','))
		{
			//Create tempPlayer object
			Player tempPlayer;
			//Insert player name from file
			tempPlayer.playerInfo.playerName = line;
			//Get next line from file
			getline(ifile, line);
			//Assign score to variable from file
			int fileScore = stoi(line);

			//if currentIndex more than 10 , stop adding player history to message
			if (currentIndex <= 10)
			{
				//if currentIndex less than or eqaul to 10, continue adding player history to message
				std::string playerHistory = tempPlayer.playerInfo.playerName + ":" + std::to_string(fileScore);
				str += "\n" + playerHistory;
			}
			//if assign score to playerscore object
			tempPlayer.playerInfo.playerScore = fileScore;
			//Push the temp Player object into the List of players
			fileInputPlayer.push_back(tempPlayer);
			//Increment Index
			currentIndex += 1;
		}
		//Close file reading
		ifile.close();
		//Init index for usage
		int index = 0;
		// set list begin and end , for loop
		for (auto i = fileInputPlayer.begin(); i != fileInputPlayer.end(); i++)
		{
			// check if player current score is more than or equal to playerhistory playerscore
			if (playerPoints >= i->playerInfo.playerScore)
			{
				//assign index for insertion
				indexToInsert = index;
				break;
			}
			//increment index
			index++;
		}
		//Assign player name and player score to insertPlayer object
		insertPlayer.playerInfo.playerName = player->playerInfo.playerName;
		insertPlayer.playerInfo.playerScore = playerPoints;
		//if no history inside the file. will add current player name and score
		if (currentIndex == 0)
		{
			str += "\n" + player->playerInfo.playerName + ":" + std::to_string(playerPoints);
		}
		//set begin of fileInputPlayer list to tempPlayer
		auto tempPlayer = fileInputPlayer.begin();
		//advance tempPlayer to the index of insertion
		advance(tempPlayer, indexToInsert);
		//insert player into the list
		fileInputPlayer.insert(tempPlayer, insertPlayer);
		//Append scoreboard.txt
		std::ofstream ofs("scoreboard.txt", std::ofstream::trunc);
		//for loop
		for (auto it = fileInputPlayer.begin(); it != fileInputPlayer.end(); it++)
		{
			//Save the player name and player score into the .txt file
			ofs << it->playerInfo.playerName << "," << it->playerInfo.playerScore << "\n";
		}
		//close writer
		ofs.close();
	}
	//error catching
	catch (...)
	{
		cout << "Something went wrong while opening the file.";
	}
	//convert string msg to char
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
	//
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
	{
		SDL_Log("error displaying message box");
		return 1;
	}
	//If no button is selected
	if (buttonid == -1)
	{
		SDL_Log("no selection");
	}
	//if selected is no
	else if (buttonid == 0)
	{
		//stop game and exit game
		return 0;
	}
	//if selected is yes
	else if (buttonid == 1)
	{
		//restart game and navigate to main menu
		return 1;
	}
	//stop game and exit game
	return 0;
}
