#pragma once

#include <vector>
using std::vector;

#include "Vector2D.h"
#include "Component.h"
#include "Tilemap.h"
#include "Grid.h"


class Map : public Tilemap {
public:
	enum Tile {
		tIllegal = -1,
		tPlains,
		// LR = cliff touching left & right edges of tile, RB = right & bottom edges, etc
		tCliffLR, tCliffTB, tCliffLT, tCliffLB, tCliffRT, tCliffRB
	};

	Map();

	Grid InitGrid();

	Component* Clone() const;

	void Update(float dt);
	void Draw() const;

	void Load(rapidjson::Value &obj);

private:
	//int width;
	//int height;
	//int *tiles;
	//Tilemap *tilemap;
};