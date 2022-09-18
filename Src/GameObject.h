#pragma once

#include "Game.h"

class GameObject {
private:
	int xpos;
	int ypos;

	SDL_Texture* objTexture;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	
public:
	GameObject(const char* textureSheet, int x, int y);
	~GameObject();

	void Update();
	void Render();
};

