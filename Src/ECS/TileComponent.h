#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

// this component is used to give entities textures, positions, and dimensions
class TileComponent : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	SDL_Rect tileRect;
	int tileID;
	const char* path;

	// allow the tile to be initialised with either nothing, or positions, dimensions, and a texture
	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id) {
		tileRect.x = x;
		tileRect.y = y;
		tileRect.w = w;
		tileRect.h = h;
		tileID = id;

		switch (tileID) {
		case 0:
			path = "assets/Holes/GrassHole.png";
			break;
		case 1:
			path = "assets/BackgroundTiles/Grass.png";
			break;
		case 2:
			path = "assets/WallTiles/Grass/wall2.png";
			break;
		default:
			break;
		}
	}

	// this function is called when the component is created
	void init() override {
		entity->addComponent<TransformComponent>(static_cast<float>(tileRect.x),
			static_cast<float>(tileRect.y), tileRect.w, tileRect.h, 1);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}
};