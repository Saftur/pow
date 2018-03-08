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

Grid Map::GetPathfinderGrid() {
	for (int y = 0; y < GetTilemapHeight(); y++) {
		for (int x = 0; x < GetTilemapWidth(); x++) {
			int tile = GetTile(x, y);
			if (tile == tPlains) {

			} else if (tile >= tCliffLR && tile <= tCliffRB) {

			}
		}
	}
	return Grid();
}

Component * Map::Clone() const {
	return new Map(*this);
}

void Map::Update(float dt) {
}

void Map::Draw() const {}

void Map::Load(rapidjson::Value & obj) {
	Tilemap::Load(obj);
	/*if (obj.HasMember("Name") && obj["Name"].IsString()) {
		ReadMap(obj["Name"].GetString());
	}*/
}
