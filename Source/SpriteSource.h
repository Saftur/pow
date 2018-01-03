//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.h
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct AEGfxTexture AEGfxTexture;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class SpriteSource
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new sprite source object.
	// Params:
	//	 numCols = The number of columns in the sprite sheet.
	//	 numRows = The number of rows in the sprite sheet.
	//	 pTexture = A pointer to a texture that has been loaded by the Alpha Engine.
	SpriteSource(int numCols, int numRows, struct AEGfxTexture* pTexture);

	// Returns a pointer to the texture, for the purposes of rendering a sprite.
	// Returns:
	//	 A pointer to the sprite source texture.
	struct AEGfxTexture* GetTexture() const;

	// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
	// (Hint: Frame count = numCols * numRows.)
	// Returns:
	//	 The calculated frame count.
	unsigned int GetFrameCount() const;

	// Returns the UV coordinates of the specified frame in a sprite sheet.
	// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
	// Params:
	//	 frameIndex = A frame index within a sprite sheet.
	//   u = Pointer to a f32 to contain the U coordinate. 
	//   v = Pointer to a f32 to contain the V coordinate. 
	void GetUV(unsigned int frameIndex, float* u, float* v) const;

private:
	// The dimensions of the sprite sheet.
	int	numRows;
	int	numCols;

	// Pointer to a texture created using the Alpha Engine.
	AEGfxTexture*	pTexture;
};
/*----------------------------------------------------------------------------*/
