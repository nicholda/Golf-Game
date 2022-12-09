#pragma once

#include "../Game.h"
#include "../Vector2D.h"
#include "ECS.h"
#include "Components.h"

class KeyboardComponent : public Component {
private:
	Vector2D mouseStartPos;
	Vector2D mouseEndPos;

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

	Vector2D getHitPower() {
		mouseEndPos = getMousePos();

		Vector2D vel;
		vel.x = mouseStartPos.x - mouseEndPos.x;
		vel.y = mouseStartPos.y - mouseEndPos.y;
		Vector2D distanceMultiplier;
		distanceMultiplier.x = 100;
		distanceMultiplier.y = 100;
		vel /= distanceMultiplier;
		vel *= -1;

		if (vel.x > 2.5) {
			vel.x = 2.5;
		} else if (vel.x < -2.5) {
			vel.x = -2.5;
		}
		if (vel.y > 2.5) {
			vel.y = 2.5;
		} else if (vel.y < -2.5) {
			vel.y = -2.5;
		}

		return vel;
	}

	Vector2D getMousePos() {
		int mouseX, mouseY;
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
		Vector2D mousePos;
		mousePos.x = static_cast<float>(mouseX);
		mousePos.y = static_cast<float>(mouseY);

		return mousePos;
	}
};