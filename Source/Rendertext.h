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

#include "Component.h"

typedef struct AEGfxVertexList AEGfxVertexList;
typedef struct AEGfxTexture AEGfxTexture;
typedef class SpriteSource SpriteSource;
typedef class Sprite Sprite;

class Text : public Component {
public:
	Text();

	void setText(const char* string); //Set the text.

	void Update(float dt); //Update the text on screen.

	void Load(rapidjson::Value& obj);

private:
	/***********************************************
	* Spritesheet Format
	*
	* 		Space
	* 		0-9
	* 		a-z
	* 		A-Z
	* 		!  #  $  (  )  :  ;  ?  ,  /  .
	*
	***********************************************/
	char string[512];
	
	AEGfxVertexList* mesh;			//Mesh component for drawing the text.
	AEGfxTexture* texture;			//Texture component for drawing the text.
	SpriteSource* spritesource;		//Spritesource component for drawing the text.
	Sprite *sprite;					//Sprite component for drawing the text.
};
