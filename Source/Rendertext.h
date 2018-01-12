#pragma once

#include "Vector2D.h"
#include "Sprite.h"
#include "Transform.h"
#include "SpriteSource.h"

class Text{
private:
	AEGfxVertexList* mesh;
	AEGfxTexture* texture;
	SpriteSource* spritesource;
	Sprite sprite;
	Transform transform;
	char string[256];

public:
	Text(const char * text, Vector2D pos, Vector2D scale);
	~Text();

	void setText(const char* string);
	void draw();
	void setLocation(Vector2D loc);
	void setScale(Vector2D scale);
};