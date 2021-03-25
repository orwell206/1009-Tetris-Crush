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

int main()
{
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
		SDL_Window *window = SDL_CreateWindow("Tetris Crush Team 42", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 780, 570, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		Player player;

		GameBoard gameboard = {};
		gameboard.set_player(player);

		InputState input = {};

		gameboard.spawn_tetromino(&gameboard);

		bool quit = false;
		//Open font
		const char *font_name = "novem___.ttf";
		TTF_Font *font = TTF_OpenFont(font_name, 24);
		while (!quit)
		{
			gameboard.set_GameBoardTime(SDL_GetTicks() / 1000.0f);

			SDL_Event e;
			while (SDL_PollEvent(&e) != 0)
			{

				if (e.type == SDL_QUIT)
				{
					quit = true;
					restart = true;
				}
			}

			s32 key_count;
			const u8 *key_states = SDL_GetKeyboardState(&key_count);

			InputState prev_input = input;

			// P1 Controls
			input.a = key_states[SDL_SCANCODE_A];
			input.d = key_states[SDL_SCANCODE_D];
			input.w = key_states[SDL_SCANCODE_W];
			input.s = key_states[SDL_SCANCODE_S];
			input.space = key_states[SDL_SCANCODE_SPACE];

			input.da = input.a - prev_input.a;
			input.dd = input.d - prev_input.d;
			input.dw = input.w - prev_input.w;
			input.ds = input.s - prev_input.s;
			input.dspace = input.space - prev_input.space;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);

			int returnValue = gameboard.update_game(&gameboard, &input, renderer, font);
			if (returnValue == 0)
			{

				quit = true;
				restart = true;
			}
			else if (returnValue == 1)
			{

				restart = false;
				quit = true;
			}
			gameboard.render_game(&gameboard, renderer, font);

			SDL_RenderPresent(renderer);
		}
		//Close font after game close
		TTF_CloseFont(font);
		SDL_DestroyRenderer(renderer);
		SDL_RenderClear(renderer);
		SDL_Quit();
	} while (!restart);
}
