#pragma once

#include "Game.h"

class TextureManager {
public:
	TextureManager() {};
	~TextureManager() {};

	// load a texture from via a file path, provides functionality for 'SpriteComponent'
	static SDL_Texture* LoadTexture(const char* fileName);
	// draw a texture to an SDL_Rect, provides functionality for 'SpriteComponent'
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};