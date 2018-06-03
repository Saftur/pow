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

#include <string>
using std::string;

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
	Text(bool manualCreation = false, const char* text = "", const char* font = "Assets\\NewFont.png",
		Color color = { 0, 0, 0, 0 }, Vector2D textScale = { 15, 15 });

	// Clones the component, returning a dynamically allocated copy.
	Component* Clone() const;

	void SetActive(bool active);
	void SetText(const char* text); //Set the text.
	void SetText(string text); //Set the text.
	void SetColor(Color color);
	void SetScale(Vector2D textScale);
	void SetOffset(Vector2D offset);

	bool GetActive() const;
	string GetText() const;
	Color GetColor() const;
	Vector2D GetScale() const;
	Vector2D GetOffset() const;

	void Draw(Camera *cam) const; //Update the text on screen.

	void Load(rapidjson::Value& obj);

protected:
	bool active;

	string str;
	Vector2D scale;
	
	AEGfxVertexList* mesh;			//Mesh component for drawing the text.
	AEGfxTexture* texture;			//Texture component for drawing the text.
	SpriteSource* spritesource;		//Spritesource component for drawing the text.
	Sprite *sprite;					//Sprite component for drawing the text.

	Vector2D offset;
};
