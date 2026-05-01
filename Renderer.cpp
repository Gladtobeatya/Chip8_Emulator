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

	return bIsRunning = true;
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
	default:
		break;
	}

	// Move paddles
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
