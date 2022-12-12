#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "ECS.h"
#include "../Game.h"

class UILabelComponent : public Component {
private:
	SDL_Rect position;
	std::string labelText;
	SDL_Color labelColor;
	SDL_Texture* labelTexture;

public:
	UILabelComponent(int xPos, int yPos, std::string text, SDL_Color& color) :
		labelText(text), labelColor(color) {
		position.x = xPos;
		position.y = yPos;

		SetLabelText(labelText);
	}
	~UILabelComponent() {}

	void SetLabelText(std::string text) {
		SDL_Surface* surf = TTF_RenderText_Blended(Game::font, text.c_str(), labelColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void SetLabelPosition(int xPos, int yPos) {
		position.x = xPos;
		position.y = yPos;
	}

	void draw() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}
};