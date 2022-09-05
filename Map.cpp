#include "Map.h"
#include "TextureManager.h"

int lvl1[20][20] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,4,7,7,7,7,7,7,5,0,0,0,0},
	{0,0,0,0,0,0,0,0,10,1,1,1,1,1,1,8,0,0,0,0},
	{0,0,0,0,0,0,0,0,10,1,1,1,1,1,1,8,0,0,0,0},
	{0,0,0,0,0,0,0,0,3,9,9,9,9,1,1,8,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,8,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,8,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,3,9,6,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

Map::Map() {
	grass = TextureManager::LoadTexture("assets/BackgroundTiles/grass.png");
	wall1 = TextureManager::LoadTexture("assets/WallTiles/grass/wall1.png");
	wall2 = TextureManager::LoadTexture("assets/WallTiles/grass/wall2.png");
	wall3 = TextureManager::LoadTexture("assets/WallTiles/grass/wall3.png");
	wall4 = TextureManager::LoadTexture("assets/WallTiles/grass/wall4.png");
	wall5 = TextureManager::LoadTexture("assets/WallTiles/grass/wall5.png");
	wall6 = TextureManager::LoadTexture("assets/WallTiles/grass/wall6.png");
	wall7 = TextureManager::LoadTexture("assets/WallTiles/grass/wall7.png");
	wall8 = TextureManager::LoadTexture("assets/WallTiles/grass/wall8.png");
	wall9 = TextureManager::LoadTexture("assets/WallTiles/grass/wall9.png");
	wall10 = TextureManager::LoadTexture("assets/WallTiles/grass/wall10.png");

	LoadMap(lvl1);

	src.x = 0;
	src.y = 0;
	src.h = 16;
	src.w = 16;

	dest.x = 0;
	dest.y = 0;
	dest.w = 16 * 2;
	dest.h = 16 * 2;
}

Map::~Map() {

}

void Map::LoadMap(int arr[20][20]) {
	for (int row = 0; row < 20; row++) {
		for (int column = 0; column < 20; column++) {
			map[row][column] = arr[row][column];
		}
	}
}

void Map::DrawMap() {
	int type = 0;

	for (int row = 0; row < 20; row++) {
		for (int column = 0; column < 20; column++) {
			type = map[row][column];

			dest.x = column * 32;
			dest.y = row * 32;

			switch (type) {
			case 0:
				TextureManager::Draw(grass, src, dest);
				break;
			case 1:
				TextureManager::Draw(wall1, src, dest);
				break;
			case 2:
				TextureManager::Draw(wall2, src, dest);
				break;
			case 3:
				TextureManager::Draw(wall3, src, dest);
				break;
			case 4:
				TextureManager::Draw(wall4, src, dest);
				break;
			case 5:
				TextureManager::Draw(wall5, src, dest);
				break;
			case 6:
				TextureManager::Draw(wall6, src, dest);
				break;
			case 7:
				TextureManager::Draw(wall7, src, dest);
				break;
			case 8:
				TextureManager::Draw(wall8, src, dest);
				break;
			case 9:
				TextureManager::Draw(wall9, src, dest);
				break;
			case 10:
				TextureManager::Draw(wall10, src, dest);
				break;
			default:
				break;
			}
		}
	}
}