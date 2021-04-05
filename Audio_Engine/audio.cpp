#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Define the path of the audio files
#define PATH_BGM "Assets/Tetris.wav"
#define PATH_SE_CLEAR "Assets/ClearSound.wav"
#define PATH_SE_FALL "Assets/FallTetris.wav"

class Audio 
{
protected: 
    Mix_Music *audioMusic;
    Mix_Chunk *audioChunk;
    bool runningStatus = false;  
    int volume = 50;
public:
    void load(Mix_Music*);      // Overloading - to load different types of audio
    void load(Mix_Chunk*);      // Overloading - to load different types of audio
    void play();
    void stop();
    void setStatus(bool);       // Encapsulation - set the audio status
    bool getStatus();           // Encapsulation - get the audio status
};

void Audio::load(Mix_Music *audio) 
{
    /*
        Desc: Load background music. 
        Params: Mix_Music (*audio)
    */
    audioMusic = audio;
    Mix_VolumeMusic(volume);    // Set the volume with default value
}

void Audio::load(Mix_Chunk *audio) 
{ 
    /*
        Desc: Load sound effect. 
        Params: Mix_Music (*audio)
    */
    audioChunk = audio;
}

void Audio::play() 
{
    /*
        Desc: Play the audio in a loop.
        Params: None.
    */
    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(audioMusic,-1);   // Put -1 so that it runs in a loop
    }
    if(Mix_PlayingMusic())
    {
        Mix_ResumeMusic();
    }
}

void Audio::stop() 
{
    /*
        Desc: Stop playing audio.
        Params: None.
    */
    Mix_HaltMusic();
}

void Audio::setStatus(bool status)
{
    /*
        Desc: Set the audio status (encapsulation)
        Params: bool (status)
    */
    runningStatus = status;
}

bool Audio::getStatus()
{
    /*
        Desc: Get the audio status (encapsulation)
        Params: None
    */
    return runningStatus;
}
