#pragma once

#include "Game.h"

class TextureManager {
public:
	TextureManager() {};
	~TextureManager() {};

	// load a texture from via a file path, provides functionality for 'SpriteComponent'
	static SDL_Texture* loadTexture(const char* texture);
	// draw a texture to an SDL_Rect, provides functionality for 'SpriteComponent'
	static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};