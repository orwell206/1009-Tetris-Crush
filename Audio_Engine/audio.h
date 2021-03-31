#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define PATH_BGM "Assets/Tetris.wav"
#define PATH_SE_GAMEOVER "Assets/GameOver.wav"
#define PATH_SE_CLEAR "Assets/ClearSound.wav"
#define PATH_SE_FALL "Assets/FallTetris.wav"

class Audio {
protected: 
    Mix_Music *audioMusic;
    Mix_Chunk *audioChunk;
    bool runningStatus = false;
    int volume = 50;     // range from 0-10
public:
    void load(Mix_Music*);       // overloading
    void load(Mix_Chunk*);       // overloading
    void play();
    void pause();
    void setStatus(bool);
    bool getStatus();
    void setVolume(int);
};

class SoundEffect: public Audio{
public:
    void play();    // override
    void setVolume(int);    // override
};

#endif //AUDIO_H