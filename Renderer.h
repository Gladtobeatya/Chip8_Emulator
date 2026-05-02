#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <array>
#include "Chip8Config.h"
#include <unordered_map>
#include <functional>


class Renderer
{
private:
	bool bIsRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Uint64 frameStart;
	Uint64 frameElapsed;
	std::unordered_map<SDL_Scancode, uint8_t> keyMap;

public:
	std::function<void(uint8_t)> evtKeyDown;
	std::function<void(uint8_t)> evtKeyUp;

	bool init();
	void initKeyMap();
	void update(const std::array<std::array<uint8_t, CHIP8_WIDTH>, CHIP8_HEIGHT>& screen);
	void handleEvents();
	bool isRunning();
	void quit();
	void waitForNextFrame();
	void tick();
};

