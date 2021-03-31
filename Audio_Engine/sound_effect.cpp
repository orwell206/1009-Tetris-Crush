#include "audio.cpp"

class SoundEffect: public Audio{
public:
    void play();    // Overriding - to play the audio once only
};

void SoundEffect::play() {
    Mix_PlayChannel(-1, audioChunk,0);  // (channel,audio,loop=0)
    Mix_HaltChannel(1);
}