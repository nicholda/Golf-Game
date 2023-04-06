#include "Map.h"
#include "Vector2.h"

Map::Map() {
	
}

Map::~Map() {
	
}

void Map::loadMap(std::string path) { // 'assets/Levels/Level_1.json' is an example map path
	std::ifstream mapFile(path);
	Json::Value actualJson;
	Json::Reader reader;
	reader.parse(mapFile, actualJson);

	int mapSizeX = actualJson["tileswide"].asInt();
	int mapSizeY = actualJson["tileshigh"].asInt();
	
	// create background tiles
	for (int i = 0; i < mapSizeX * mapSizeY; i++) {
		Json::Value tile = actualJson["layers"][1]["tiles"][i];
		int tileType = tile["tile"].asInt();
		if (tileType != -1) { // tileType -1 is an empty tile
			Vector2 tilePos;
			tilePos.x = tile["x"].asInt();
			tilePos.y = tile["y"].asInt();
			
			Game::addTile(tileType, tilePos.x * 32, tilePos.y * 32, false);
		}
	}
	
	// create collidable tiles
	for (int i = 0; i < mapSizeX * mapSizeY; i++) {
		Json::Value tile = actualJson["layers"][0]["tiles"][i];
		int tileType = tile["tile"].asInt();
		if (tileType != -1) { // tileType -1 is an empty tile
			Vector2 tilePos;
			tilePos.x = tile["x"].asInt();
			tilePos.y = tile["y"].asInt();
			
			Game::addTile(tileType, tilePos.x * 32, tilePos.y * 32, true);
		}
	}
}