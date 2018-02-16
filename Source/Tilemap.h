#pragma once

#include "Component.h"
#include "Vector2D.h"

typedef class Sprite Sprite;
typedef class SpriteSource SpriteSource;
typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class Transform Transform;

class Tilemap : public Component {
public:
	Tilemap(const char* spritesheetFilename, const char* tilemapFilename, const char* collisionMapFilename,
		int onScreenOffsetX, int onScreenOffsetY, int onScreenWidth, int onScreenHeight,
		int spritesheetWidth, int spritesheetHeight);
	Tilemap();
	~Tilemap();
	Component* Clone() const;
	void Draw() const;
	Vector2D GetPosOnMap(Vector2D screenPos, Vector2D *offsetFromTile = nullptr) const;
	Vector2D GetPosOnScreen(Vector2D tilePos) const;
	bool IsObjectCollidingWithMap(Vector2D objectPosition, Vector2D objectScale) const;
	void SetOffset(int onScreenOffsetX, int onScreenOffsetY);

	int GetTileWidth() const;
	int GetTileHeight() const;
	int GetTilemapWidth() const;
	int GetTilemapHeight() const;

private:
	void ReadFiles(const char* tilemapFilename, const char* collisionMapFilename);

	int GetTilenum(int x, int y) const;
	int GetCollides(int x, int y) const;

	void Load(rapidjson::Value& obj);

	int offsetX, offsetY; // on-screen offset from top-left
	int width, height;    // on-screen width/height
	int tileWidth, tileHeight; // on-screen size of tiles
	int *tilemap;         // 2d array of tile nums
	int tilemapWidth, tilemapHeight; // width/height of tilemap
	bool *collisionMap;   // 2d array of colliding sprites

	Sprite* sprite;
	SpriteSource* spriteSource;
	AEGfxTexture* texture;
	AEGfxVertexList* meshQuad;
	Transform* transform;
};