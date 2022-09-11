#pragma once

#include "../Game.h"
#include "../Vector2D.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component {
private:
	Vector2D mouseStartPos;
	Vector2D mouseEndPos;

	Vector2D getMousePos() {
		int mouseX, mouseY;
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
		Vector2D mousePos;
		mousePos.x = mouseX;
		mousePos.y = mouseY;

		return mousePos;
	}

	void mousePress(SDL_MouseButtonEvent& input) {
		if (input.button == SDL_BUTTON_LEFT) {
			mouseStartPos = getMousePos();
		}
	}

	void mouseRelease(SDL_MouseButtonEvent& input) {
		if (input.button == SDL_BUTTON_LEFT) {
			mouseEndPos = getMousePos();

			Vector2D vel = mouseStartPos - mouseEndPos;
			Vector2D distanceMultiplier;
			distanceMultiplier.x = 100;
			distanceMultiplier.y = 100;
			vel /= distanceMultiplier;

			std::cout << vel << "\n";

			transform->velocity.x = -vel.x;
			transform->velocity.y = -vel.y;
		}
	}

public:
	TransformComponent* transform;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
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