#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;

auto& ball(manager.addEntity());

Game::Game() {

}

Game::~Game() {

}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (window) {
			std::cout << "Window created!\n";
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!\n";
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}
	map = new Map();

	ball.addComponent<PositionComponent>(0, 0);
	ball.addComponent<SpriteComponent>("assets/Balls/Ball.png");
}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

void Game::update() {
	manager.refresh();
	manager.update();

	if (ball.getComponent<PositionComponent>().x() > 100) {
		ball.getComponent<SpriteComponent>().setTex("assets/Balls/GoldBall.png");
	}
}

bool Game::running() {
	return isRunning;
}

void Game::render() {
	SDL_RenderClear(renderer);
	map->DrawMap();
	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}