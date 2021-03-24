#g++ -std=c++17 main.cpp -o rMac -I "SDL2/include_mac" -L "SDL2/lib_mac" -lSDL2 -lSDL2_ttf
g++ -std=c++17 main.cpp -o rMac -lSDL2main -lSDL2 -lSDL2_ttf -lstdc++
./rMac
