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
bool Game::finished = false;
Mix_Chunk* Game::puttSound;
Mix_Chunk* Game::holeSound;
Mix_Chunk* Game::wallSound;

// used to differentiate between different types of entities
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

// this function is called when the game is created via 'main.cpp'
void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) { // if false then there's an error initialising
		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (window) {
			std::cout << "Window created!\n";
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 160, 190, 70, 255);
			std::cout << "Renderer created!\n";
		}

		std::cout << "SDL_Mixer successfully initialized!\n";

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (Mix_Init(0) == 0) { // SDL_Mixer is used for audio handling
		std::cout << "SDL_Mixer successfully initialized!\n";
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	puttSound = Mix_LoadWAV("assets/Sounds/Putt Sound.wav");
	holeSound = Mix_LoadWAV("assets/Sounds/Hole Sound.wav");
	wallSound = Mix_LoadWAV("assets/Sounds/Wall Hit Sound.wav");

	if (TTF_Init() == 0) {// SDL_TTF is used for text
		std::cout << "SDL_TTF successfully initialized!\n";
	}

	font = TTF_OpenFont("assets/Fonts/arial.ttf", 28); // load arial
	if (font) {
		std::cout << "Font loaded!\n";
	}

	map = new Map(); // allocate the map to the heap

	Map::LoadMap("assets/Levels/Level_1.json"); // load the first level

	// setup the ball
	ball.addComponent<TransformComponent>(0.0f, 0.0f, 32, 32, 0.5f);
	ball.addComponent<SpriteComponent>("assets/Balls/Ball.png");
	ball.addComponent<KeyboardComponent>();
	ball.addComponent<ColliderComponent>("ball");
	ball.addGroup(groupBalls);

	SDL_Color darkBlue = { 0, 0, 250, 255 }; // dark blue is used because it contrasts the background
	scoreLabel.addComponent<UIComponent>(275, 5, "Score: ", darkBlue);

	// setup the power metre that is next to the ball
	powerMetre.addComponent<TransformComponent>(0.0f, 0.0f, 12, 8, 1.0f);
	powerMetre.addComponent<SpriteComponent>("assets/UI/Red.png");
	powerMetre.addGroup(groupUi);

	TransformComponent* ballTransform = &ball.getComponent<TransformComponent>();
	ballTransform->velocity.Zero();
	// the same ball position is used at the start of every level
	ballTransform->position.x = 320;
	ballTransform->position.y = 600;
}

// this function handles player input
void Game::handleEvents() {
	SDL_PollEvent(&event);

	// switch has been used to allow for added functionality
	switch (event.type) {
	case SDL_QUIT: // called when the exit button is pressed at the top of the window
		isRunning = false;
		break;

	default:
		break;
	}
}

void Game::update() {
	// update the score counter before the manager is refreshed
	std::stringstream ss;
	ss << "Score: " << score;
	scoreLabel.getComponent<UIComponent>().SetLabelText(ss.str());

	manager.refresh();

	if (Game::finished) { // when the game is finished, the game doesn't need to update
		return;
	}

	TransformComponent* ballTransform = &ball.getComponent<TransformComponent>();
	ColliderComponent* ballCollider = &ball.getComponent<ColliderComponent>();

	Vector2 prevBallPos;
	prevBallPos.x = ballTransform->position.x;
	prevBallPos.y = ballTransform->position.y;

	manager.update(); // update positions after storing previous positions

	// ensure that the golf ball stays within the bounds of the window
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

	// iterate through the colliders
	bool wallHit = false;
	for (int i = 0; i < colliders.size(); i++) {
		auto cc = colliders[i];

		// if i is not colliding with the ball continue
		if (!Collision::AABB(*ballCollider, *cc, ballTransform->position)) {
			continue;
		}

		if (cc->tag == "hole") { // if ball hits hole load new level
			if (ballTransform->velocity.magnitude() > 0.3) { // if ball is going too fast continue
				continue;
			}
			Mix_PlayChannel(-1, Game::holeSound, 0);
			Game::level += 1;
			auto& tilesGroup(manager.getGroup(groupMap));
			for (int j = 0; j < colliders.size(); j++) { // clear the level
				auto cc2 = colliders[j];
				if (cc2->tag != "ball") {
					cc2->entity->delGroup(groupMap);
					cc2->entity->destroy();
					//TODO: test to see if this causes the memory leak
				}
			}
			colliders.clear();
			if (Game::level < 8) {
				std::string first = "assets/Levels/Level_";
				std::string last = ".json";
				Map::LoadMap(first + std::to_string(Game::level) + last);
				hits = 0;
				ballTransform->velocity.Zero();
				ballTransform->position.x = 320;
				ballTransform->position.y = 600;
				score += 100;
			} else { // game complete
				Game::finished = true;
				ball.delGroup(groupBalls);
				ball.destroy();
				font = TTF_OpenFont("assets/Fonts/arial.ttf", 64);
				UIComponent* labelComponent = &scoreLabel.getComponent<UIComponent>();
				labelComponent->SetLabelPosition(210, 250);
			}
		} else { // if ball hits wall destroy wall
			Mix_PlayChannel(-1, Game::wallSound, 0);
			if (!wallHit) { // if the ball has already rebounded this frame, don't do it again
				Collision::Rebound(*ballCollider, *cc, ballTransform, prevBallPos);
			}
			
			cc->entity->delGroup(groupMap);
			cc->entity->destroy();
			colliders.erase(colliders.begin() + i);
			wallHits += 1;
			score += 5 * wallHits;
		}
		
		wallHit = true; // stored so the ball doesn't rebound multiple times
	}

	// render the correct power metre dimensions
	TransformComponent* powerMetreTransform = &powerMetre.getComponent<TransformComponent>();
	KeyboardComponent* keyboardComponent = &ball.getComponent<KeyboardComponent>();
	if (Game::hitting) {
		Vector2 hitPower = keyboardComponent->getHitPower();
		hitPower.x = abs(hitPower.x);
		hitPower.y = abs(hitPower.y);
		powerMetreTransform->height = std::max(hitPower.x, hitPower.y) * 15;
	} else {
		powerMetreTransform->height = 0;
	}
	powerMetreTransform->position.x = ballTransform->position.x - 25;
	powerMetreTransform->position.y = 
		ballTransform->position.y - powerMetreTransform->height + ballTransform->height / 2;

	if (score < 0) {
		score = 0; // ensure that the score can't become negative
	}
}

bool Game::running() {
	return isRunning;
}

auto& tilesGroup(manager.getGroup(Game::groupMap));
auto& ballsGroup(manager.getGroup(Game::groupBalls));
auto& UiGroup(manager.getGroup(Game::groupUi));

// renders in groups so that the ball is ontop of walls and walls are on top of background
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

	scoreLabel.draw(); // ui should render after all entities

	SDL_RenderPresent(renderer);
}

void Game::clean() { // clean up the game after it's closed
	TTF_CloseFont(font);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// this function adds tiles, not balls
void Game::AddTile(int id, int x, int y, bool collidable) {

	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);

	if (collidable) {
		if (id == 0) {
			tile.addComponent<ColliderComponent>("hole");
		} else { // if another tile type is added this should use else if
			tile.addComponent<ColliderComponent>("wall");
		}
	}

	tile.addGroup(groupMap); // add to the group so that it can be rendered in groups
}