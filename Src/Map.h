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

	static void loadMap(std::string path); // 'assets/Levels/Level_1.json' is an example map path
};