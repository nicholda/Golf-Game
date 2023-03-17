#pragma once

#include "Components.h"
#include "../TextureManager.h"
#include <map>
#include "SDL.h"

// this component is used by used to allow entities to draw textures relative to their position
class SpriteComponent : public Component {
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

public:
	// this variable is obsolete but has been kept in case of an update
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	// allow the class to be initialised using a path to a texture or no texture
	SpriteComponent() = default;
	SpriteComponent(const char* path) {
		setTex(path);
	}

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}

	// sets the texture to a path relative to the executable file
	void setTex(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	// this function is called when the component is created
	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	// this function is called every frame
	void update() override {

		//srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);
		destRect.w = static_cast<int>(transform->width * transform->scale);
		destRect.h = static_cast<int>(transform->height * transform->scale);
	}

	// this function is called from 'Game.cpp'
	void draw() override {
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}
};