#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <array>
#include "Chip8Config.h"


class Renderer
{
private:
	bool bIsRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Uint64 frameStart;
	Uint64 frameElapsed;

public:
	bool init();
	void update(const std::array<std::array<uint8_t, CHIP8_WIDTH>, CHIP8_HEIGHT>& screen);
	void handleEvents();
	bool isRunning();
	void quit();
	void waitForNextFrame();
	void tick();
};

