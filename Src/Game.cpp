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

	Map::LoadMap("assets/Levels/Level_1.json");

	ball.addComponent<TransformComponent>(0.0f, 0.0f, 32, 32, 0.5f);
	ball.addComponent<SpriteComponent>("assets/Balls/Ball.png");
	ball.addComponent<KeyboardController>();
	ball.addComponent<ColliderComponent>("ball");
	ball.addGroup(groupBalls);
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

	TransformComponent* ballTransform = &ball.getComponent<TransformComponent>();

	for (auto cc : colliders) {
		bool hit = Collision::AABB(ball.getComponent<ColliderComponent>(), *cc);
		if (hit) {
			Collision::reboundBall(ball.getComponent<ColliderComponent>(), *cc, ballTransform);
		}
	}

	// make the ball stop if going too slow instead of velocity = 0.000000000001
	ballTransform->velocity *= 0.98;
	if (abs(ballTransform->velocity.x) <= 0.05 && abs(ballTransform->velocity.y) <= 0.05) {
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

void Game::AddTile(int id, int x, int y, bool collidable) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);

	if (collidable) {
	tile.addComponent<ColliderComponent>("wall");
	}

	tile.addGroup(groupMap);
}