#include "Game.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Game::Game() {

}

Game::~Game() {

}

void Game::init(const char* title, int x, int y, int w, int h, const int windowFlags, const int rendererFlags) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "error initializing SDL: &s\n", SDL_GetError();
		running = false;

	}
	else {

		window = SDL_CreateWindow(title, x, y, w, h, windowFlags);

		if (window) {
			std::cout << "window created\n";
		}
		else {
			std::cout << "error creating window: &s\n", SDL_GetError();
		}
		
		renderer = SDL_CreateRenderer(window, -1, rendererFlags);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "renderer created\n";
		} else {
			std::cout << "error creating renderer: &s\n", SDL_GetError();
		}

		running = true;
	}
}

void Game::handleEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;

		default:
			break;
		}
	}
}

void Game::update() {

}

void Game::render() {
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

bool Game::isRunning() {
	return running;
}

void Game::clean() {
	std::cout << "cleaning SDL\n";

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}