#pragma once

#include <string>
#include <json/value.h>
#include <json/json.h>
#include <fstream>
#include "Game.h"

class Map {
public:
	Map();
	~Map();

	static void LoadMap(std::string path);
};