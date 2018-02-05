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
	static AEGfxTexture *textureUnit;
	static AEGfxTexture *textureAbility;
	static AEGfxVertexList *meshUnit;
	static SpriteSource *spriteSourceUnit;
	static SpriteSource* spriteSourceAbility;
	static AEGfxVertexList *meshAbility;

	static Army *army1;
	static Army *army2;

	static vector<Vector2D> path1;
	static vector<Vector2D> path2;

	static int frontLine1;
	static int frontLine2;

	static void CreateUnit(Army &army, const char *name, Vector2D pos, vector<Vector2D> path = {});
	static bool LegalSpawn(Army &army, Vector2D pos);
};
/*----------------------------------------------------------------------------*/
