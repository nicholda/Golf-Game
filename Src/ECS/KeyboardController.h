#pragma once

#include "../Game.h"
#include "../Vector2D.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component {
private:
	bool hitting = false;
	Vector2D mouseStartPos;
	Vector2D mouseEndPos;

	Vector2D getMousePos() {
		int mouseX, mouseY;
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
		Vector2D mousePos;
		mousePos.x = static_cast<float>(mouseX);
		mousePos.y = static_cast<float>(mouseY);

		return mousePos;
	}

	void mousePress(SDL_MouseButtonEvent& input) {
		if (input.button == SDL_BUTTON_LEFT && transform->velocity.x == 0 && transform->velocity.y == 0) {
			mouseStartPos = getMousePos();
			hitting = true;
		}
	}

	void mouseRelease(SDL_MouseButtonEvent& input) {
		if (input.button == SDL_BUTTON_LEFT && hitting == true) {
			mouseEndPos = getMousePos();

			Vector2D vel = mouseStartPos - mouseEndPos;
			Vector2D distanceMultiplier;
			distanceMultiplier.x = 100;
			distanceMultiplier.y = 100;
			vel /= distanceMultiplier;

			transform->velocity.x = -vel.x;
			transform->velocity.y = -vel.y;

			hitting = false;
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
};