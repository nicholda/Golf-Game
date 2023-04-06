#include "TextureManager.h"

// load a texture from via a file path, provides functionality for 'SpriteComponent'
SDL_Texture* TextureManager::loadTexture(const char* texture) {
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

// draw a texture to an SDL_Rect, provides functionality for 'SpriteComponent'
void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}