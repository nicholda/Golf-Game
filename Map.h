#pragma once

#include "Game.h"

class Map
{
private:
	SDL_Rect src, dest;

	SDL_Texture* grass;
	SDL_Texture* wall1;
	SDL_Texture* wall2;
	SDL_Texture* wall3;
	SDL_Texture* wall4;
	SDL_Texture* wall5;
	SDL_Texture* wall6;
	SDL_Texture* wall7;
	SDL_Texture* wall8;
	SDL_Texture* wall9;
	SDL_Texture* wall10;

	int map[20][20];

public:
	Map();
	~Map();

	void LoadMap(int arr[20][20]);
	void DrawMap();
};