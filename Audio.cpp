#include "Audio.h"

void Audio::init()
{
    SDL_AudioSpec spec{};

    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 2048;

    spec.callback = Audio::handleAudio;
    spec.userdata = this;

    device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    SDL_PauseAudioDevice(device, 0);
}

void Audio::update(bool soundOn)
{
    beepOn = soundOn;
}

void Audio::shutdown()
{
    SDL_CloseAudioDevice(device);
}

void Audio::handleAudio(void* userdata, Uint8* stream, int len)
{
    Audio* audio = static_cast<Audio*>(userdata);

    Sint16* buffer = reinterpret_cast<Sint16*>(stream);
    int samples = len / 2;

    for (int i = 0; i < samples; i++) {

        if (audio->beepOn) {
            audio->phase += audio->phaseIncrement;
            if (audio->phase >= 1.0) audio->phase -= 1.0;

            buffer[i] = (audio->phase < 0.5) ? 1000 : -1000;
        }
        else {
            buffer[i] = 0;
        }
    }
}