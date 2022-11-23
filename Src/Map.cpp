#include "Map.h"
#include "Vector2D.h"

Map::Map() {
	
}

Map::~Map() {
	
}

void Map::LoadMap(std::string path) {
	std::ifstream mapFile(path);
	Json::Value actualJson;
	Json::Reader reader;
	reader.parse(mapFile, actualJson);

	int mapSizeX = actualJson["tileswide"].asInt();
	int mapSizeY = actualJson["tileshigh"].asInt();
	
	// place background tiles
	for (int i = 0; i < mapSizeX * mapSizeY; i++) {
		Json::Value tile = actualJson["layers"][1]["tiles"][i];
		int tileType = tile["tile"].asInt();
		if (tileType != -1) {
			int x = tile["x"].asInt();
			int y = tile["y"].asInt();
			Game::AddTile(tileType, x * 32, y * 32, false);
		}
	}
	
	// place collidable tiles
	for (int i = 0; i < mapSizeX * mapSizeY; i++) {
		Json::Value tile = actualJson["layers"][0]["tiles"][i];

		int tileType = tile["tile"].asInt();
		if (tileType != -1) {
			Vector2D tilePos;
			tilePos.x = tile["x"].asInt();
			tilePos.y = tile["y"].asInt();

			Game::AddTile(tileType, tilePos.x * 32, tilePos.y * 32, true);
		}
	}
}	