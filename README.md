# 1009-Tetris-Crush-C-

### Project Description 
A Tetris game that is coded using C++ and is coded with the use of Object Oriented Projects (OOP). 

### Tool Name
Discuss and come up with a catchy name for your application,
- `Tetris Crush 1009`
-----

**Must have:**
- [x] Tetris board
- [x] Score board 
- [x] Multiple Tetris block 
- [x] Ability to rotate Tetris block
- [x] Separate engines: Game engine, Player Engine, Audio Engine, UI Engine
- [x] Visually pleasing
- [x] GUI 

##### Should have:
- [ ] Main menu
- [ ] Game can be paused

##### Could have:
- [X] Soundtrack
- [ ] Multiple levels
- [X] Smooth animation/Effect

##### Would have:
- [ ] Settings menu
- [X] Persistent scoreboard

### Instructions 
1. Run the following command to compile the program into “main.exe”
g++ -std=c++17 main.cpp -I "SDL2\include" -L "SDL2\lib" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lstdc++ -o main

2. Run the following command to start the program
main.exe
