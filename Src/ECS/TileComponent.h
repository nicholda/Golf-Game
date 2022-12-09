#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

class TileComponent : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	SDL_Rect tileRect;
	int tileID;
	const char* path;

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

	void init() override {
		entity->addComponent<TransformComponent>(static_cast<float>(tileRect.x),
			static_cast<float>(tileRect.y), tileRect.w, tileRect.h, 1);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}
};