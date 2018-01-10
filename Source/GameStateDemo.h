//------------------------------------------------------------------------------
//
// File Name:	GameStateDemo.h
// Author(s):	Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
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

class Tilemap;
typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
class GameStateDemo
{
public:
	//------------------------------------------------------------------------------
	// Public Consts:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Load the resources associated with the Demo game state.
	static void Load();

	// Initialize the memory associated with the Demo game state.
	static void Init();

	// Update the Demo game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Update(float dt);

	// Shutdown any memory associated with the Demo game state.
	static void Shutdown();

	// Unload the resources associated with the Demo game state.
	static void Unload();

private:
	static Tilemap *tilemap;
	static AEGfxTexture *textureHex;
	static AEGfxVertexList *meshQuad;
	static Sprite *spriteHex;
	static SpriteSource *spriteSourceHex;
	static Transform *transformHex;
};
/*----------------------------------------------------------------------------*/