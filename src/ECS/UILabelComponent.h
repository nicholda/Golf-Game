#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "ECS.h"
#include "../Game.h"

// this component is used to allow entities to render text
class UILabelComponent : public Component {
private:
	SDL_Rect position;
	std::string labelText;
	SDL_Color labelColor;
	SDL_Texture* labelTexture;

public:
	// allow the class to be initialised with a position, text and colour
	UILabelComponent(int xPos, int yPos, std::string text, SDL_Color& color) :
		labelText(text), labelColor(color) {
		position.x = xPos;
		position.y = yPos;

		SetLabelText(labelText);
	}
	~UILabelComponent() {}

	void SetLabelText(std::string text) {
		SDL_Surface* surf = TTF_RenderText_Blended(Game::font, text.c_str(), labelColor);
		//SDL_Texture* labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);

		// retrieve texture settings
		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);

		SDL_FreeSurface(surf); // frees the resources used by surf
		//SDL_DestroyTexture(labelTexture);
	}

	void SetLabelPosition(int xPos, int yPos) {
		position.x = xPos;
		position.y = yPos;
	}

	// this function is called from 'Game.cpp'
	void draw() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}
};