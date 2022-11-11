#include "Map.h"

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
			Game::AddTile(tileType, x * 32, y * 32);
		}
	}
	
	/*char tile;
	std::fstream mapFile;
	mapFile.open(path);

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(tile);
			Game::AddTile(atoi(&tile), x * 32, y * 32);
			mapFile.ignore();
		}
	}

	mapFile.close();*/
}