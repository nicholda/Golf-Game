#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "ECS.h"
#include "../Game.h"

// this component is used to allow entities to render text
class UIComponent : public Component {
private:
	SDL_Rect position;
	std::string labelText;
	SDL_Color labelColor;
	SDL_Texture* labelTexture;

public:
	// allow the class to be initialised with a position, text and colour
	UIComponent(int xPos, int yPos, std::string text, SDL_Color& color) :
		labelText(text), labelColor(color) {
		position.x = xPos;
		position.y = yPos;

		setLabelText(labelText);
	}
	~UIComponent() {}

	void setLabelText(std::string text) {
		if (text == labelText) { // no point rendering the same thing
			return;
		}
		labelText = text;

		SDL_Surface* surf = TTF_RenderText_Blended(Game::font, labelText.c_str(), labelColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);

		// retrieve texture settings
		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);

		SDL_FreeSurface(surf); // frees the resources used by surf
	}

	void setLabelPosition(int xPos, int yPos) {
		position.x = xPos;
		position.y = yPos;
	}

	// this function is called from 'Game.cpp'
	void draw() override {
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}
};