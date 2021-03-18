// gcc -std=c++17 main.cpp -I "C:\Users\tux\Documents\SIT\ICT1009\Projects\C++ Project\SDL2\include" -L "C:\Users\tux\Documents\SIT\ICT1009\Projects\C++ Project\SDL2\lib" -Wall -lmingw32 -lSDL2main -lSDL2 -o main
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cassert>
#include "player.cpp"
#include "colours.hpp"
#include "gameboard.cpp"
#include "input_state.cpp"


int main(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("Tetris Crush Team 42", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 780, 570, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Player player;


	GameBoard gameboard = {};
	gameboard.set_player(player);

	InputState input = {};

	gameboard.spawn_tetromino(&gameboard);

	bool quit = false;
	while (!quit){
		gameboard.time = SDL_GetTicks() / 1000.0f;


		SDL_Event e;
		while (SDL_PollEvent(&e) != 0){
			if (e.type == SDL_QUIT){
				quit = true;
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

		gameboard.update_game(&gameboard, &input);
		gameboard.render_game(&gameboard, renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}