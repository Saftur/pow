//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
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
#include "Sprite.h"
#include "SpriteSource.h"
#include "Trace.h"
#include "Transform.h"
#include "Mesh.h"

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct AEGfxVertexList AEGfxVertexList;
typedef struct AEVec2 AEVec2;

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

// Dynamically allocate a new sprite object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (NOTE: Make sure to initialize the 'alpha' member variable to 1.0f.)
// Params:
//	 name = The name of the sprite.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Sprite::Sprite(const char * name)
	: name(name), alpha(1.0f), spriteSource(nullptr) {}

// Draw a sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the sprite object.
//   position = The world position of the sprite.
void Sprite::Draw(Transform& transform)
{
	if (spriteSource != NULL)
	{
		// Calculate UV offset
		float u, v;

		spriteSource->GetUV(frameIndex, &u, &v);

		// Draw sprite
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(spriteSource->GetTexture(), u, v);
	}
	else
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}

	AEGfxSetTransparency(alpha);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

// Adjust a sprite's alpha up/down.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f.)
// Params:
//	 sprite = Pointer to the sprite object.
//   alphaAdjust = Offset to apply to the sprite's 'alpha' value.
void Sprite::AdjustAlpha(float alphaAdjust)
{
	if (this->alpha + alphaAdjust > 1.0f)
		this->alpha = 1.0f;
	else if (this->alpha + alphaAdjust < 0.0f)
		this->alpha = 0.0f;
	else
		this->alpha += alphaAdjust;
}

// Set the sprite's current frame.
// (NOTE: You must validate the frame index against the sprite source's frame count.)
// Params:
//	 sprite = Pointer to the sprite object.
//   frameIndex = New frame index for the sprite (0 .. frame count).
// ADDITIONAL REQUIREMENTS:
//   This function must make the following function call:
//     TraceMessage("SpriteSetFrame: %s frame index = %d", this->name, frameIndex);
void Sprite::SetFrame(unsigned int frameIndexPain)
{
	// Validate frameIndex against sprite source's frame count
	if (frameIndexPain < 0 || frameIndexPain >= spriteSource->GetFrameCount())
		return;

	// Set the frame index
	frameIndex = frameIndexPain;

	// Print debug message
	Trace::GetInstance().GetStream() << "SpriteSetFrame: " << this->name << " frame index = " << this->frameIndex;
}

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the sprite object.
//   mesh = Pointer to a mesh created using the Alpha Engine.
void Sprite::SetMesh(AEGfxVertexList * meshA)
{
	mesh = meshA;
}

// Set a new SpriteSource for the specified sprite.
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 spriteSource = A new sprite source for the sprite.
void Sprite::SetSpriteSource(SpriteSource* spriteSourceWhy)
{
	spriteSource = spriteSourceWhy;
}

/*----------------------------------------------------------------------------*/
