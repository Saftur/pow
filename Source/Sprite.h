//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
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

typedef struct AEGfxVertexList AEGfxVertexList;
typedef class Transform Transform;
typedef class SpriteSource SpriteSource;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class Sprite
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new sprite object.
	// (NOTE: Make sure to initialize the 'alpha' member variable to 1.0f.)
	// Params:
	//	 name = The name of the sprite.
	Sprite(const char * name);

	// Draw a sprite (Sprite can be textured or untextured).
	// Params:
	//   transform = Reference to the object's transform.
	void Draw(Transform& transform);

	// Adjust a sprite's alpha up/down.
	// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f.)
	// Params:
	//   alphaAdjust = Offset to apply to the sprite's 'alpha' value.
	void AdjustAlpha(float alphaAdjust);

	// Set the sprite's current frame.
	// (NOTE: You must validate the frame index against the sprite source's frame count.)
	// Params:
	//   frameIndex = New frame index for the sprite (0 .. frame count).
	// ADDITIONAL REQUIREMENTS:
	//   This function must make the following function call:
	//     Trace::GetInstance().GetStream() << "SpriteSetFrame: %s frame index = %d", sprite->name, frameIndex);
	void SetFrame(unsigned int frameIndex);

	// Set the sprite's mesh.
	// (NOTE: This mesh may be textured or untextured.)
	// (NOTE: This mesh may contain any number of triangles.)
	// Params:
	//   mesh = Pointer to a mesh created using the Alpha Engine.
	void SetMesh(AEGfxVertexList* mesh);

	// Set a new SpriteSource for the specified sprite.
	// Params:
	//	 spriteSource = A new sprite source for the sprite.
	void SetSpriteSource(SpriteSource* spriteSource);

private:
	// The name of the sprite.
	const char * name;

	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (nullptr = simple colored mesh).
	SpriteSource* spriteSource;

	// The mesh used to draw the sprite.
	AEGfxVertexList * mesh;
};
/*----------------------------------------------------------------------------*/
