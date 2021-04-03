#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cassert>
#include "Player_Engine/player.cpp"
#include "Player_Engine/input_state.cpp"
#include "Game_Engine/gameboard.cpp"
#include "Game_Engine/gameboard_graphics.cpp"
#include "time.h"

#define WIDTH 10
#define HEIGHT 19
#define PLAYABLE_HEIGHT 16
#define GRID_SIZE 30
#define TEXT_HORZ_ALIGN (WIDTH * GRID_SIZE + 30)
#define TEXT_VERT_ALIGN HEIGHT
enum TextAlign
{
	TEXTALIGNLEFT,
	TEXTALIGNCENTER,
	TEXTALIGNRIGHT
};
string playerName;
int runGameBoard(string playerName)
{
	// Seed random
	srand(time(NULL));

	//Init Audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		return -1;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		return -1;
	}
	// Play background music
	Mix_Music *bgmusic = Mix_LoadMUS(PATH_BGM);
	Audio bgm;
	bgm.load(bgmusic);
	bgm.play();
	bgm.setStatus(true);

	// SDL_Window *window = SDL_CreateWindow("Tetris Crush Team 42", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 780, 570, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_Window *window = SDL_CreateWindow("Tetris Crush Team 42", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 570, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//Create player
	Player player;
	//Set playerName
	player.playerInfo.playerName = playerName;
	//Init Gameboard class
	GameBoard gameboard{};
	//Set gameboard with player
	gameboard.set_player(player);

	InputState input = {};
	//Spawn tetromino
	gameboard.spawn_tetromino(&gameboard);

	//Init declaration of quit
	bool quit = false;

	// Open font
	const char *font_name = "novem___.ttf";
	TTF_Font *font = TTF_OpenFont(font_name, 24);
	//Keep looping if condition is true
	while (!quit)
	{

		gameboard.set_GameBoardTime(SDL_GetTicks() / 1000.0f);

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//Capture force close of game window
			if (e.type == SDL_QUIT)
			{
				quit = true;
				// Close font and render after game close
				TTF_CloseFont(font);
				SDL_DestroyRenderer(renderer);
				SDL_RenderClear(renderer);
				// Close SDL mixer
				Mix_CloseAudio();
				SDL_Quit();
				//return to main method
				return 1;
			}
		}

		s32 key_count;
		const u8 *key_states = SDL_GetKeyboardState(&key_count);

		InputState prev_input = input;

		input.set_a(key_states[SDL_SCANCODE_A]);
		input.set_d(key_states[SDL_SCANCODE_D]);
		input.set_w(key_states[SDL_SCANCODE_W]);
		input.set_s(key_states[SDL_SCANCODE_S]);
		input.set_space(key_states[SDL_SCANCODE_SPACE]);

		input.set_da(input.get_a() - prev_input.get_a());
		input.set_dd(input.get_d() - prev_input.get_d());
		input.set_dw(input.get_w() - prev_input.get_w());
		input.set_ds(input.get_s() - prev_input.get_s());
		input.set_dspace(input.get_space() - prev_input.get_space());

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		int returnValue = gameboard.update_game(&gameboard, &input, renderer, font);
		if (returnValue == 0)
		{
			// Stop playing background music
			if (bgm.getStatus())
			{
				bgm.stop();
				bgm.setStatus(false);
			}

			quit = true;
			// Close font and render after game close
			TTF_CloseFont(font);
			SDL_DestroyRenderer(renderer);
			SDL_RenderClear(renderer);
			// Close SDL mixer
			Mix_CloseAudio();
			SDL_Quit();
			//return to main method
			return 1;
		}
		else if (returnValue == 1)
		{
			// Close font and render after game close
			TTF_CloseFont(font);
			SDL_DestroyRenderer(renderer);
			SDL_RenderClear(renderer);
			// Close SDL mixer
			Mix_CloseAudio();
			SDL_Quit();
			//return to main method
			return 0;
			quit = true;
		}
		gameboard.render_game(&gameboard, renderer, font);

		SDL_RenderPresent(renderer);
	}
	return 1;
}
int renderText()
{
	const char *font_name = "novem___.ttf";
	TTF_Font *font = TTF_OpenFont(font_name, 24);
	SDL_Window *window = SDL_CreateWindow("Tetris Crush Team 42", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 570, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	s32 x = 350 * 0.7;
	s32 y = 80;
	Color highlight_color = Color(0xFF, 0xFF, 0xFF, 0xFF);
	GameBoardGraphics graph{};
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set text color as black
	SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF};

	//The current input text.
	std::string inputText = "";
	graph.loadFromRenderedText(renderer, font, inputText.c_str(), textColor);

	//Enable text input
	SDL_StartTextInput();
	s32 key_count2;
	const u8 *key_states2 = SDL_GetKeyboardState(&key_count2);
	//While application is running
	while (!quit)
	{
		//The rerender text flag
		bool renderText = false;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{

			if (e.type == SDL_QUIT)
			{
				//close render and return to main method
				quit = true;
				SDL_StopTextInput();
				SDL_DestroyRenderer(renderer);
				SDL_RenderClear(renderer);
				TTF_CloseFont(font);
				SDL_Quit();
				return 0;
			}
			if (key_states2[SDL_SCANCODE_RETURN])
			{
				//when input text not none
				if (inputText != "")
				{
					//close render and return to main method
					playerName = inputText;
					SDL_StopTextInput();
					SDL_DestroyRenderer(renderer);
					SDL_RenderClear(renderer);
					TTF_CloseFont(font);
					SDL_Quit();
					return 1;
				}
			}
			//Special key input
			if (e.type == SDL_KEYDOWN)
			{
				//Handle backspace
				if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
				{
					//lop off character
					inputText.pop_back();
					renderText = true;
				}
				//Handle copy
				else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
				{
					SDL_SetClipboardText(inputText.c_str());
				}
				//Handle paste
				else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
				{
					inputText = SDL_GetClipboardText();
					renderText = true;
				}
			}
			//Special text input event
			else if (e.type == SDL_TEXTINPUT)
			{
				//Not copy or pasting
				if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
				{
					//Append character
					inputText += e.text.text;
					renderText = true;
				}
			}
		} //Rerender text if needed
		if (renderText)
		{
			//Text is not empty
			if (inputText != "")
			{
				//Render new text
				graph.loadFromRenderedText(renderer, font, inputText.c_str(), textColor);
			}
			//Text is empty
			else
			{
				//Render space texture
				graph.loadFromRenderedText(renderer, font, " ", textColor);
			}
		} //Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		//Render text textures
		graph.draw_text(renderer, font, "TETRIS CRUSH", x, y, TEXT_ALIGN_CENTER, highlight_color);
		graph.draw_text(renderer, font, "Enter Player Name", x, y * 2.5 - 70, TEXT_ALIGN_CENTER, highlight_color);
		graph.draw_text(renderer, font, "before you start game", x, y * 2.5 - 40, TEXT_ALIGN_CENTER, highlight_color);
		graph.draw_text(renderer, font, "Play", x, y * 5, TEXT_ALIGN_CENTER, highlight_color);
		graph.draw_text(renderer, font, "Press Enter to start game", x, y * 4, TEXT_ALIGN_CENTER, highlight_color);
		graph.renderText(renderer, x - 50, y * 2.5);

		//Update screen
		SDL_RenderPresent(renderer);
	}
	return 0;
}
int main()
{
	//Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return -1;
	}
	//For TTF font
	if (TTF_Init() < 0)
	{
		return 2;
	}

	bool restart = false;
	do
	{
		//Render text on SDL and get return value
		int renderValue = renderText();
		if (renderValue != 0)
		{
			//Run gameboard and get return value when application close/user selected restart/exit
			int returnValueFromGameBoard = runGameBoard(playerName);
			if (returnValueFromGameBoard == 1)
			{
				restart = true;
			}
			else if (returnValueFromGameBoard == 0)
			{

				restart = false;
			}
		}
		else
		{
			restart = true;
		}

	} while (!restart);

	return 0;
}
