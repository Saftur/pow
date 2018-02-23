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

#include "Component.h"
#include "Vector2D.h"

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

// Currently used only for blend color
struct Color
{
	float r;
	float g;
	float b;
	float a;
};

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class Sprite : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new sprite object.
	// (NOTE: Make sure to initialize the 'alpha' member variable to 1.0f.)
	Sprite();

	// Returns a dynamically allocated copy of the component.
	// Must be implemented so correct component is copied during copy.
	Component* Clone() const;

	// Draw a sprite (Sprite can be textured or untextured).
	void Draw() const;
	void Draw(Transform &transform) const;

	// Adjust a sprite's alpha up/down.
	// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f.)
	// Params:
	//   alphaAdjust = Offset to apply to the sprite's 'alpha' value.
	void AdjustAlpha(float alphaAdjust);

	void SetAlpha(float alpha);

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

	//void SetMeshHalfSize(Vector2D halfSize);
	//void SetMeshUV(Vector2D UV);

	// Set a new SpriteSource for the specified sprite.
	// Params:
	//	 spriteSource = A new sprite source for the sprite.
	void SetSpriteSource(SpriteSource* spriteSource);

	// Set the blend/tint color for the specified sprite.
	// Params:
	//	 color = A new color for the sprite.
	void SetModulateColor(Color color);

	void Load(rapidjson::Value& obj);


	static void SetBounds(Vector2D topLeft, Vector2D bottomRight);

private:
	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (nullptr = simple colored mesh).
	SpriteSource* spriteSource;

	// The mesh used to draw the sprite.
	AEGfxVertexList * mesh;
	//Vector2D meshHalfSize;
	//Vector2D meshUV;

	// Color used for blending/tint
	Color color;

	//AEGfxVertexList* CutMesh(Transform &transform) const;

	static Vector2D topLeftBound;
	static Vector2D bottomRightBound;
};
/*----------------------------------------------------------------------------*/
