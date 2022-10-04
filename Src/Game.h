#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Game {
private:
	bool running{};
	SDL_Window* window{};

public:
	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, int windowFlags, int rendererFlags);
	void handleEvents();
	void update();
	void render();
	bool isRunning();
	void clean();

	static SDL_Renderer* renderer;
	static SDL_Event event;
};