#include <sstream>
#include <cmath>
#include <cassert>
#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2.h"
#include "Collision.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
TTF_Font* Game::font = nullptr;
SDL_Event Game::event;
int Game::score;
int Game::hits = 0;
int Game::wallHits = 0;
int Game::level = 1;
bool Game::hitting;
int Game::hitPower;
Mix_Chunk* Game::puttSound;
Mix_Chunk* Game::holeSound;
Mix_Chunk* Game::wallSound;

enum Game::groupLabels : std::size_t {
	groupMap,
	groupBalls,
	groupUi
};

std::vector<ColliderComponent*> Game::colliders;

Entity& ball(manager.addEntity());
Entity& powerMetre(manager.addEntity());
Entity& scoreLabel(manager.addEntity());

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

		std::cout << "SDL_Mixer successfully initialized!\n";

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (Mix_Init(0) == 0) {
		std::cout << "SDL_Mixer successfully initialized!\n";
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	puttSound = Mix_LoadWAV("assets/Sounds/Putt Sound.wav");
	holeSound = Mix_LoadWAV("assets/Sounds/Hole Sound.wav");
	wallSound = Mix_LoadWAV("assets/Sounds/Wall Hit Sound.wav");

	if (TTF_Init() == 0) {
		std::cout << "SDL_TTF successfully initialized!\n";
	}

	font = TTF_OpenFont("assets/Fonts/arial.ttf", 28);
	if (font) {
		std::cout << "Font loaded!\n";
	}

	map = new Map();

	Map::LoadMap("assets/Levels/Level_1.json");

	ball.addComponent<TransformComponent>(0.0f, 0.0f, 32, 32, 0.5f);
	ball.addComponent<SpriteComponent>("assets/Balls/Ball.png");
	ball.addComponent<KeyboardComponent>();
	ball.addComponent<ColliderComponent>("ball");
	ball.addGroup(groupBalls);

	SDL_Color darkBlue = { 0, 0, 250, 255 };
	scoreLabel.addComponent<UILabelComponent>(275, 5, "Score: 0", darkBlue);

	powerMetre.addComponent<TransformComponent>(0.0f, 0.0f, 16, 8, 1.0f);
	powerMetre.addComponent<SpriteComponent>("assets/UI/Red.png");
	powerMetre.addGroup(groupUi);

	TransformComponent* ballTransform = &ball.getComponent<TransformComponent>();
	ballTransform->velocity.Zero();
	ballTransform->position.x = 320;
	ballTransform->position.y = 600;
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
	std::stringstream ss;
	ss << "Score: " << score;
	scoreLabel.getComponent<UILabelComponent>().SetLabelText(ss.str());

	manager.refresh();

	TransformComponent* ballTransform = &ball.getComponent<TransformComponent>();
	ColliderComponent* ballCollider = &ball.getComponent<ColliderComponent>();

	Vector2 prevBallPos;
	prevBallPos.x = ballTransform->position.x;
	prevBallPos.y = ballTransform->position.y;

	manager.update(); // update positions after storing previous positions

	Vector2 ballPos;
	ballPos.x = ballTransform->position.x;
	ballPos.y = ballTransform->position.y;

	if (ballTransform->position.x < 0) {
		Mix_PlayChannel(-1, Game::wallSound, 0);
		ballTransform->position.x = 0;
		ballTransform->velocity.x *= -1;
	} else if (ballTransform->position.x > 640 - ballCollider->collider.w) {
		Mix_PlayChannel(-1, Game::wallSound, 0);
		ballTransform->position.x = 640 - ballCollider->collider.w;
		ballTransform->velocity.x *= -1;
	}
	if (ballTransform->position.y < 0) {
		Mix_PlayChannel(-1, Game::wallSound, 0);
		ballTransform->position.y = 0;
		ballTransform->velocity.y *= -1;
	} else if (ballTransform->position.y > 640 - ballCollider->collider.h) {
		Mix_PlayChannel(-1, Game::wallSound, 0);
		ballTransform->position.y = 640 - ballCollider->collider.h;
		ballTransform->velocity.y *= -1;
	}

	// make the ball stop if going too slow instead of velocity = 0.000000000001
	ballTransform->velocity *= 0.985;
	if (abs(ballTransform->velocity.x) <= 0.05 && abs(ballTransform->velocity.y) <= 0.05) {
		ballTransform->velocity.Zero();
	}

	for (int i = 0; i < colliders.size(); i++) {
		auto cc = colliders[i];

		if (!Collision::AABB(*ballCollider, *cc, ballPos)) {
			continue;
		}

		if (cc->tag == "hole") { // if ball hits hole load new level
			Mix_PlayChannel(-1, Game::holeSound, 0);
			Game::level += 1;
			if (Game::level == 16) {
				std::cout << score << "\n";
				isRunning = false;
			}
			auto& tilesGroup(manager.getGroup(groupMap));
			for (int j = 0; j < colliders.size(); j++) {
				auto cc2 = colliders[j];
				if (cc2->tag != "ball") {
					cc2->entity->delGroup(groupMap);
					cc2->entity->destroy();
				}
			}
			colliders.clear();
			std::string first = "assets/Levels/Level_";
			std::string last = ".json";
			Map::LoadMap(first + std::to_string(Game::level) + last);
			hits = 0;
			ballTransform->velocity.Zero();
			ballTransform->position.x = 320;
			ballTransform->position.y = 600;
			score += 100;
		} else { // if ball hits wall destroy wall
			Mix_PlayChannel(-1, Game::wallSound, 0);
			Collision::rebound(*ballCollider, *cc, ballTransform, prevBallPos);
				
			cc->entity->delGroup(groupMap);
			cc->entity->destroy();
			colliders.erase(colliders.begin() + i);
			wallHits += 1;
			score += 5 * wallHits;
		}
		break;
	}

	TransformComponent* powerMetreTransform = &powerMetre.getComponent<TransformComponent>();
	KeyboardComponent* keyboardComponent = &ball.getComponent<KeyboardComponent>();
	if (Game::hitting) {
		Vector2 hitPower = keyboardComponent->getHitPower();
		hitPower.x = abs(hitPower.x);
		hitPower.y = abs(hitPower.y);
		powerMetreTransform->height = std::max(hitPower.x, hitPower.y) * 10;
	} else {
		powerMetreTransform->height = 0;
	}
	powerMetreTransform->position.x = ballTransform->position.x - 25;
	powerMetreTransform->position.y = 
		ballTransform->position.y - powerMetreTransform->height + ballTransform->height / 2;

	if (score < 0) {
		score = 0;
	}
}

bool Game::running() {
	return isRunning;
}

auto& tilesGroup(manager.getGroup(Game::groupMap));
auto& ballsGroup(manager.getGroup(Game::groupBalls));
auto& UiGroup(manager.getGroup(Game::groupUi));

void Game::render() {
	SDL_RenderClear(renderer);
	for (auto& t : tilesGroup) {
		t->draw();
	}
	for (auto& b : ballsGroup) {
		b->draw();
	}
	for (auto& u : UiGroup) {
		u->draw();
	}

	scoreLabel.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	TTF_CloseFont(font);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::AddTile(int id, int x, int y, bool collidable) {

	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);

	if (collidable) {
		if (id == 0) {
			tile.addComponent<ColliderComponent>("hole");
		} else {
			tile.addComponent<ColliderComponent>("wall");
		}
	}

	tile.addGroup(groupMap);
}