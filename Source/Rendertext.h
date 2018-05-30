////------------------------------------------------------------------------------
//// File Name:	Rendertext.h
//// Author(s):	Brand Knutson
//// Project:		Push of War
//// Course:		Gam150
////
//// Copyright © 2017 DigiPen (USA) Corporation.
////------------------------------------------------------------------------------
//
// Copyright © 2017 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#pragma once

#include "Sprite.h"
#include "Vector2D.h"
#include "Component.h"

typedef struct AEGfxVertexList AEGfxVertexList;
typedef struct AEGfxTexture AEGfxTexture;
typedef class SpriteSource SpriteSource;
typedef class Sprite Sprite;
typedef struct Color color;
typedef class Vector2D Vector2D;

class Text : public Component {
public:
	Text();

	Text(const Text &other);

	// Clones the component, returning a dynamically allocated copy.
	Component* Clone() const;

	void SetText(const char* string); //Set the text.
	void SetColor(Color color);
	void SetScale(Vector2D textScale);
	void SetOffset(Vector2D offset);

	void Draw(Camera *cam) const; //Update the text on screen.

	void Load(rapidjson::Value& obj);

private:
	char string[512];
	Vector2D scale;
	
	Sprite *sprite;					//Sprite component for drawing the text.

	Vector2D offset;
};
