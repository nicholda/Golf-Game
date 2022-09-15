#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;

auto& ball(manager.addEntity());
auto& wall(manager.addEntity());

auto& tile0(manager.addEntity());
auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());

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

	tile0.addComponent<TileComponent>(200, 200, 32, 32, 0);
	tile0.addComponent<ColliderComponent>("grass");
	tile1.addComponent<TileComponent>(250, 250, 32, 32, 1);
	tile1.addComponent<ColliderComponent>("grassy_grass");
	tile2.addComponent<TileComponent>(300, 300, 32, 32, 2);
	tile2.addComponent<ColliderComponent>("ice");

	ball.addComponent<TransformComponent>();
	ball.addComponent<SpriteComponent>("assets/Balls/Ball.png");
	ball.addComponent<KeyboardController>();
	ball.addComponent<ColliderComponent>("ball");

	wall.addComponent<TransformComponent>(300.0f, 300.0f, 32, 32, 1);
	wall.addComponent<SpriteComponent>("assets/WallTiles/Grass/wall2.png");
	wall.addComponent<ColliderComponent>("wall");
}

void Game::handleEvents() {
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

	for (auto cc : colliders) {
		Collision::AABB(ball.getComponent<ColliderComponent>(), *cc);
	}

	ball.getComponent<TransformComponent>().velocity *= 0.98;
}

bool Game::running() {
	return isRunning;
}

void Game::render() {
	SDL_RenderClear(renderer);
	//map->DrawMap();
	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}