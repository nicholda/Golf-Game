#pragma once

#include "Components.h"
#include "../Vector2.h"

// this component is used to give entities positions and dimensions for when they are rendered
class TransformComponent : public Component {
public:
	Vector2 position;
	Vector2 velocity;

	// default values
	int height = 32;
	int width = 32;
	float scale = 1;
	int speed = 3;
	
	// allow the component to be initialised with various parameters
	TransformComponent() {
		position.Zero();
	}

	TransformComponent(float sc) {
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int w, int h, float sc) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	// this function is called when the component is created
	void init() override {
		velocity.Zero();
	}

	// this function is called every frame
	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
		//position.x += static_cast<float>(rand() % 100) / 100 - 0.5 + velocity.x * speed;
		//position.y += static_cast<float>(rand() % 100) / 100 - 0.5 + velocity.y * speed;
	}
};