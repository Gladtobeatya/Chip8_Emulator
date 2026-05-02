#include "Renderer.h"
#include <iostream>

bool Renderer::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("Chip-8 Emulation by Tanguy Fournand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set focus
	SDL_RaiseWindow(window);

	return bIsRunning = true;
}

void Renderer::initKeyMap()
{
	keyMap[SDL_SCANCODE_1] = 0x1;
	keyMap[SDL_SCANCODE_2] = 0x2;
	keyMap[SDL_SCANCODE_3] = 0x3;
	keyMap[SDL_SCANCODE_4] = 0xC;

	keyMap[SDL_SCANCODE_Q] = 0x4;
	keyMap[SDL_SCANCODE_W] = 0x5;
	keyMap[SDL_SCANCODE_E] = 0x6;
	keyMap[SDL_SCANCODE_R] = 0xD;

	keyMap[SDL_SCANCODE_A] = 0x7;
	keyMap[SDL_SCANCODE_S] = 0x8;
	keyMap[SDL_SCANCODE_D] = 0x9;
	keyMap[SDL_SCANCODE_F] = 0xE;

	keyMap[SDL_SCANCODE_Z] = 0xA;
	keyMap[SDL_SCANCODE_X] = 0x0;
	keyMap[SDL_SCANCODE_C] = 0xB;
	keyMap[SDL_SCANCODE_V] = 0xF;
}

void Renderer::update(const std::array<std::array<uint8_t, CHIP8_WIDTH>, CHIP8_HEIGHT>& screen)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	for (int i = 0; i < CHIP8_HEIGHT; i++) {
		for (int j = 0; j < CHIP8_WIDTH; j++) {
			SDL_Rect rect = { j * SCALE, i * SCALE, SCALE, SCALE };
			if(screen[i][j] == BLACK)
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			else
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &rect);

		}
	}
	SDL_RenderPresent(renderer);
}

void Renderer::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		bIsRunning = false;
		break;
	case SDL_KEYDOWN:
		if(keyMap.contains(event.key.keysym.scancode) && evtKeyDown){
			evtKeyDown(keyMap[event.key.keysym.scancode]);
		}
		break;
	case SDL_KEYUP:
		if(keyMap.contains(event.key.keysym.scancode) && evtKeyUp) {
			evtKeyUp(keyMap[event.key.keysym.scancode]);
		}
		break;
	default:
		break;
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	// Escape key to quit
	if (state[SDL_SCANCODE_ESCAPE]) {
		bIsRunning = false;
	}
}

bool Renderer::isRunning()
{
	return bIsRunning;
}

void Renderer::quit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Renderer::waitForNextFrame()
{
	frameElapsed = SDL_GetTicks64() - frameStart;
	if (FRAME_DELAY > frameElapsed) {
		SDL_Delay(FRAME_DELAY - frameElapsed);
	}
}

void Renderer::tick()
{
	frameStart = SDL_GetTicks64();
}
