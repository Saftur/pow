//------------------------------------------------------------------------------
//
// File Name:	GameStateDemo.h
// Author(s):	Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include <vector>
using std::vector;

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Tilemap;
class Army;
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

	static Tilemap *tilemap;

private:
	// TODO Remove test vars
	static AEGfxTexture *textureHex;
	static AEGfxTexture *textureAbility;
	static AEGfxVertexList *meshQuad;
	static Sprite *spriteHex;
	static SpriteSource *spriteSourceHex;
	static Transform *transformHex;
	// End test vars

	static AEGfxTexture *textureUnit;
	static AEGfxVertexList *meshUnit;
	static SpriteSource *spriteSourceUnit;
	static SpriteSource* spriteSourceAbility;
	static AEGfxVertexList *meshAbility;

	static Army *army1;

	static void CreateUnit(Army &army, const char *name, Vector2D pos, vector<Vector2D> path);
};
/*----------------------------------------------------------------------------*/
