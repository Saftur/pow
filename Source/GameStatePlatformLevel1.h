//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.h
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

typedef class LevelTimer LevelTimer;
typedef class GameObject GameObject;
typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class SpriteSource SpriteSource;
typedef class Sprite Sprite;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
class GameStatePlatformLevel1
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

	// Load the resources associated with the Stub game state.
	static void Load();

	// Initialize the memory associated with the Stub game state.
	static void Init();

	// Update the Stub game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Update(float dt);

	static void UpdateAO(float dt);

	// Shutdown any memory associated with the Stub game state.
	static void Shutdown();

	// Unload the resources associated with the Stub game state.
	static void Unload();

	//Create the player at the given location.
	static void CreatePlayer(Vector2D loc);

	//Create a goal at the given location that goes to the target level.
	static void CreateGoal(Vector2D loc, int targetLevel);

	//Create a checkpoint at the given location.
	static void CreateCheckpoint(Vector2D loc);

private:
	static LevelTimer* timer;

	static AEGfxTexture* texturePlayer;
	static AEGfxVertexList* meshQuad;
	static SpriteSource* spriteSourcePlayer;

	static AEGfxTexture* textureGoal;
	static AEGfxVertexList* meshGoal;
	static SpriteSource* spriteSourceGoal;

	static AEGfxTexture* textureCheckpoint;
	static AEGfxVertexList* meshCheckpoint;
	static SpriteSource* spriteSourceCheckpoint;
};
/*----------------------------------------------------------------------------*/
