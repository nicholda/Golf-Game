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

enum groupLabels : std::size_t {
	groupMap,
	groupBalls,
	groupColliders
};

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

	Map::LoadMap("assets/Levels/Level_1.map", 20, 20);

	ball.addComponent<TransformComponent>(0.0f, 0.0f, 32, 32, 0.5f);
	ball.addComponent<SpriteComponent>("assets/Balls/Ball.png");
	ball.addComponent<KeyboardController>();
	ball.addComponent<ColliderComponent>("ball");
	ball.addGroup(groupBalls);

	wall.addComponent<TransformComponent>(300.0f, 300.0f, 32, 32, 1.0f);
	wall.addComponent<SpriteComponent>("assets/WallTiles/Grass/wall2.png");
	wall.addComponent<ColliderComponent>("wall");
	wall.addGroup(groupMap);
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

	TransformComponent* ballTransform = &ball.getComponent<TransformComponent>();
	ballTransform->velocity *= 0.98;
	if (ballTransform->velocity.x <= 0.05 && ballTransform->velocity.y <= 0.05) {
		ballTransform->velocity.Zero();
	}
}

bool Game::running() {
	return isRunning;
}

auto& tiles(manager.getGroup(groupMap));
auto& balls(manager.getGroup(groupBalls));

void Game::render() {
	SDL_RenderClear(renderer);
	for (auto& t : tiles) {
		t->draw();
	}
	for (auto& b : balls) {
		b->draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::AddTile(int id, int x, int y) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);
	tile.addGroup(groupMap);
}