#pragma once
#include "Component.h"
#include "SpriteSource.h"
#include "Vector2D.h"

class Tilemap : public Component
{
public:
	// Tile struct.
	struct Tile
	{
		const char* name;
		SpriteSource* image;
		bool passable;

	};

	Tilemap();
	Tilemap(Vector2D pixelSize, Vector2D gridSize);

private:
};