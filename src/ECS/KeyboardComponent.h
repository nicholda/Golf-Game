#pragma once

#include "../Game.h"
#include "../Vector2.h"
#include "ECS.h"
#include "Components.h"

// this component is used by balls to handle player keyboard and mouse input
class KeyboardComponent : public Component {
private:
	Vector2 mouseStartPos;
	Vector2 mouseEndPos;

	// this function is called every time the player presses left click down
	void mousePress(SDL_MouseButtonEvent& input) {
		// returns if already moving so that the ball can only be hit when stationary
		if (input.button == SDL_BUTTON_LEFT && transform->velocity.x == 0 && transform->velocity.y == 0) {
			mouseStartPos = getMousePos();
			Game::hitting = true;
		}
	}

	// this function is called every time the player releases left click
	void mouseRelease(SDL_MouseButtonEvent& input) {
		// returns if the player is not hitting the ball
		if (input.button == SDL_BUTTON_LEFT && Game::hitting == true) {
			transform->velocity = getHitPower();

			Game::hitting = false;
			Game::hits += 1;
			Game::wallHits = 0;
			if (Game::hits > 5) { // the amount that the score is reduced scales with the amount of hits
				Game::score -= 50;
			} else {
				Game::score -= 10 * Game::hits;
			}
			Mix_PlayChannel(-1, Game::puttSound, 0);
		}
	}

public:
	TransformComponent* transform{};
	SpriteComponent* sprite;

	// this function is called when the component is created
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	// this function is called every frame
	void update() override {
		if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
			mousePress(Game::event.button);
		}

		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			mouseRelease(Game::event.button);
		}
	}

	// this functionn returns the velocity that should be applied to a ball after it gets hit
	Vector2 getHitPower() {
		mouseEndPos = getMousePos();

		// get the velocity based on the how far the mouse moved
		Vector2 vel;
		vel.x = mouseStartPos.x - mouseEndPos.x;
		vel.y = mouseStartPos.y - mouseEndPos.y;
		Vector2 distanceMultiplier;
		distanceMultiplier.x = 100;
		distanceMultiplier.y = 100;
		vel /= distanceMultiplier; // divide the velocity by 100 so that the ball doesn't go too fast
		vel *= -1; // the velocity is currently inverted so inverse it

		// set a maximum velocity whilst keeping the direction accurate
		if (abs(vel.x) > 3 or abs(vel.y) > 3) {
			float div = std::max(abs(vel.x), abs(vel.y)) / 3.0f;
			vel.x /= div;
			vel.y /= div;
		}

		return vel;
	}

	// this function returns the mouse x and y coordinates
	Vector2 getMousePos() {
		int mouseX, mouseY;
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
		Vector2 mousePos;
		mousePos.x = static_cast<float>(mouseX);
		mousePos.y = static_cast<float>(mouseY);

		return mousePos;
	}
};