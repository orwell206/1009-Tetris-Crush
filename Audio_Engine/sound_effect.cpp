#include "audio.cpp"

class SoundEffect: public Audio{
public:
    void play();    // override
};

void SoundEffect::play() {
    // play on channel -1, which is the first free channel
    Mix_PlayChannel(-1, audioChunk,0);  
    Mix_HaltChannel(1);
}