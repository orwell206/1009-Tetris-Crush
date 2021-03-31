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
#include "time.h"

int main()
{
	// Seed random
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return -1;
	}
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return -1;
    }
	//For TTF font
	if (TTF_Init() < 0)
	{
		return 2;
	}

    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0)
    {
        return -1;
    }

	bool restart = false;
	do
	{
		// SDL_Window *window = SDL_CreateWindow("Tetris Crush Team 42", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 780, 570, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		SDL_Window *window = SDL_CreateWindow("Tetris Crush Team 42", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 570, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		Player player;

		GameBoard gameboard{};
		gameboard.set_player(player);

		InputState input = {};

		gameboard.spawn_tetromino(&gameboard);

		bool quit = false;
		//Open font
		const char *font_name = "novem___.ttf";
		TTF_Font *font = TTF_OpenFont(font_name, 24);

        // play background music
        Mix_Music *bgmusic = Mix_LoadMUS(PATH_BGM);
        Audio bgm;
        bgm.load(bgmusic);
        bgm.play();

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
