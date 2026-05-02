#pragma once
#include <SDL.h>

class Audio {
public:
    void init();
    void update(bool soundOn);
    void shutdown();

private:
    bool beepOn = false;

    double phase = 0.0;
    double phaseIncrement = 440.0 / 44100.0;

    SDL_AudioDeviceID device = 0;

    static void handleAudio(void* userdata, Uint8* stream, int len);
};

