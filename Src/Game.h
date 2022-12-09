#pragma once

#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

class ColliderComponent;

class Game {
private:
	bool isRunning{};
	SDL_Window* window{};

public:
	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running();
	void render();
	void clean();

	static void AddTile(int id, int x, int y, bool collidable);
	static SDL_Renderer* renderer;
	static TTF_Font* font;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static int level;
	static int score;
	static int hits;
	static int wallHits;
	static bool hitting;
	static int hitPower;
	static Mix_Chunk* puttSound;
	static Mix_Chunk* holeSound;
	static Mix_Chunk* wallSound;

	static enum groupLabels : std::size_t {
		groupMap,
		groupBalls,
		groupUi
	};
};