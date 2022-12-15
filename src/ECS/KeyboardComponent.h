#pragma once

#include "../Game.h"
#include "../Vector2.h"
#include "ECS.h"
#include "Components.h"

class KeyboardComponent : public Component {
private:
	Vector2 mouseStartPos;
	Vector2 mouseEndPos;

	void mousePress(SDL_MouseButtonEvent& input) {
		if (input.button == SDL_BUTTON_LEFT && transform->velocity.x == 0 && transform->velocity.y == 0) {
			mouseStartPos = getMousePos();
			Game::hitting = true;
		}
	}

	void mouseRelease(SDL_MouseButtonEvent& input) {
		if (input.button == SDL_BUTTON_LEFT && Game::hitting == true) {
			transform->velocity = getHitPower();

			Game::hitting = false;
			Game::hits += 1;
			Game::wallHits = 0;
			if (Game::hits > 5) {
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

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
			mousePress(Game::event.button);
		}

		if (Game::event.type == SDL_MOUSEBUTTONUP) {
			mouseRelease(Game::event.button);
		}
	}

	Vector2 getHitPower() {
		mouseEndPos = getMousePos();

		Vector2 vel;
		vel.x = mouseStartPos.x - mouseEndPos.x;
		vel.y = mouseStartPos.y - mouseEndPos.y;
		Vector2 distanceMultiplier;
		distanceMultiplier.x = 100;
		distanceMultiplier.y = 100;
		vel /= distanceMultiplier;
		vel *= -1;

		if (abs(vel.x) > 1.75 && abs(vel.y) > 1.75) {
			float div = std::max(abs(vel.x), abs(vel.y)) / 1.75f;
			vel.x /= div;
			vel.y /= div;
		} else {
			if (vel.x > 1.75) {
				vel.x = 1.75;
			} else if (vel.x < -1.75) {
				vel.x = -2;
			}
			if (vel.y > 1.75) {
				vel.y = 1.75;
			} else if (vel.y < -1.75) {
				vel.y = -1.75;
			}
		}

		return vel;
	}

	Vector2 getMousePos() {
		int mouseX, mouseY;
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
		Vector2 mousePos;
		mousePos.x = static_cast<float>(mouseX);
		mousePos.y = static_cast<float>(mouseY);

		return mousePos;
	}
};