#include "stdafx.h"
#include "Map.h"

#include "GameObject.h"

Map::Map() {
	name = "Map";
}

/*int Map::GetTile(int x, int y) const {
	if (x < 0 || x >= width || y < 0 || y >= height) return tIllegal;
	return tiles[y * width + x];
}*/

GridManager Map::InitGrid() {
	GridManager grid = GridManager::GetInstance();
	for (int y = 0; y < GetTilemapHeight(); y++) {
		for (int x = 0; x < GetTilemapWidth(); x++) {
			int tile = GetTile(x, y);
			if (tile == tPlains) {
				grid.SetNode(x, y, true);
			} else if (tile >= tCliffLR && tile <= tCliffRB) {
				grid.SetNode(x, y, false);
			}
		}
	}
	return grid;
}

Component * Map::Clone() const {
	return new Map(*this);
}

void Map::Update(float dt) {
}

void Map::Draw(Camera *cam) const {}

void Map::Load(rapidjson::Value & obj) {
	Tilemap::Load(obj);
	/*if (obj.HasMember("Name") && obj["Name"].IsString()) {
		ReadMap(obj["Name"].GetString());
	}*/
}
