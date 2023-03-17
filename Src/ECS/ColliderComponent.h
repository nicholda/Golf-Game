#pragma once

#include <string>
#include "SDL.h"
#include "Components.h"

// this component gives an entity a collider that can be used to see if an entity collides with any other
// entities that also have colliders
class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;
	TransformComponent* transform;

	// allow the class to be initialised using a tag 't' that is used as a name
	ColliderComponent(std::string t) {
		tag = t;
	}

	// this function is called when the component is created
	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		Game::colliders.push_back(this);
	}

	// this function is called every frame
	void update() override {
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = static_cast<int>(transform->width * transform->scale);
		collider.h = static_cast<int>(transform->height * transform->scale);
	}
};