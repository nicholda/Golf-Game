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
		position.zero();
	}

	TransformComponent(float sc) {
		position.zero();
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
		velocity.zero();
	}

	// this function is called every frame
	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};