#include "audio.h"

void Audio::load(Mix_Music *audio) {
    audioMusic = audio;
    Mix_VolumeMusic(volume);    // set default value for volume
}

void Audio::load(Mix_Chunk *audio) {
    audioChunk = audio;
}

void Audio::play() {
    if(!Mix_PlayingMusic()){
        Mix_PlayMusic(audioMusic,-1);   // put -1 so that it runs in a loop
    }
    if(Mix_PlayingMusic()){
        Mix_ResumeMusic();
    }
    runningStatus = true;
}

void Audio::pause() {
    Mix_PauseMusic();
    runningStatus = false;
}

void Audio::setStatus(bool status){
    runningStatus = status;
}

bool Audio::getStatus(){
    return runningStatus;
}

void Audio::setVolume(int volume){
    //SDL_MixAudio(Uint8 *dst, Uint8 *src, Uint32 len, int volume);
    if (volume>100) {
        this->volume = 100;  // if volume is greater than 100, set volume to 100
    } else if (volume<0) {
        this->volume = 30;  // if volume is negative, take default volume
    } else {
        this->volume = volume;
    }  

    //printf("volume was    : %d\n", Mix_VolumeMusic(this->volume));
    //printf("volume is now : %d\n", Mix_VolumeMusic(-1));

    
}


void SoundEffect::play() {
    Mix_PlayChannel(-1, audioChunk,0);   // play on channel -1 = first free channel
    Mix_HaltChannel(1);
}

void SoundEffect::setVolume(int volume){
    if (volume>100) {
        this->volume = 100;  // if volume is greater than 100, set volume to 100
    } else if (volume<0) {
        this->volume = 30;  // if volume is negative, take default volume
    } else {
        this->volume = volume;
    }  
    
    Mix_VolumeChunk(audioChunk,volume);
    //cout << Mix_VolumeChunk(audioChunk,volume) <<endl;
}