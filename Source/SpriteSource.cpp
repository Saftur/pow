//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.c
// Author(s):	Mark Culp
// Project:		CS230 Project 2
// Course:		CS230C17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "SpriteSource.h"
#include "document.h"
#include "LevelManager.h"

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct AEGfxTexture AEGfxTexture;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new sprite source object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 pTexture = A pointer to a texture that has been loaded by the Alpha Engine.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
// Create a new SpriteSource (dynamic memory allocation).
SpriteSource::SpriteSource(int numCols, int numRows, struct AEGfxTexture* pTexture, LevelManager *levelManager)
	: Serializable("SpriteSource"), numCols(numCols), numRows(numRows), pTexture(pTexture), levelManager(levelManager) {}

// Returns a pointer to the texture, for the purposes of rendering a sprite.
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return a pointer to its texture,
//		else return NULL.
struct AEGfxTexture * SpriteSource::GetTexture() const
{
	return pTexture;
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// (Hint: Frame count = numCols * numRows.)
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return the calculated frame count,
//		else return 0.
unsigned int SpriteSource::GetFrameCount() const
{
	return numCols * numRows;
}

// Returns the UV coordinates of the specified frame in a sprite sheet.
// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
// Params:
//	 spriteSource = Pointer to the sprite source object.
//	 frameIndex = A frame index within a sprite sheet.
//   u = Pointer to a f32 to contain the U coordinate. 
//   v = Pointer to a f32 to contain the V coordinate. 
void SpriteSource::GetUV(unsigned int frameIndex, float * u, float * v) const
{
	if (!numCols)
		return;
	unsigned int framesPerRow = numCols;
	unsigned int framesTotal = GetFrameCount();

	(*u) = (1.0f / framesPerRow) * (frameIndex % framesPerRow);
	(*v) = 1.0f / (framesTotal / framesPerRow) * (frameIndex / framesPerRow);
}

void SpriteSource::Load(rapidjson::Value& obj)
{
	AEGfxTexture* tex = levelManager->GetTexture(obj["Texture"].GetString());

	if (!tex)
	{
		tex = AEGfxTextureLoad(obj["Texture"].GetString());

		if (obj.HasMember("Cols"))
			numCols = obj["Cols"].GetInt();
		else numCols = 1;
		if (obj.HasMember("Rows"))
			numRows = obj["Rows"].GetInt();
		else numRows = 1;

		// Add the texture to the map.
		levelManager->AddTexture(obj["Name"].GetString(), tex);
	}
	pTexture = tex;
}

/*----------------------------------------------------------------------------*/



