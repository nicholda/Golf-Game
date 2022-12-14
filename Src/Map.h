#pragma once

#include <string>
#include <json/value.h>
#include <json/json.h>
#include <fstream>
#include "Game.h"
#include "ECS/Components.h"

class Map {
public:
	Map();
	~Map();

	static void LoadMap(std::string path);
	//static void UnloadMap(std::vector<Entity*> tilesGroup, std::vector<ColliderComponent*>& colliders);
};