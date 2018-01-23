//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project1
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class SpriteSource SpriteSource;
typedef class Sprite Sprite;
typedef class Animation Animation;
typedef class GameObject GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
class GameStateLevel1
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

	// Load the resources associated with the Level1 game state.
	static void Load();

	// Initialize the memory associated with the Level1 game state.
	static void Init();

	// Update the Level1 game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Update(float dt);

	static void UpdateAO(float dt);

	// Shutdown any memory associated with the Level1 game state.
	static void Shutdown();

	// Unload the resources associated with the Level1 game state.
	static void Unload();

private:
	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------
	// Create a game object that uses the Monkey sprite source.
	// Returns:
	//	 A pointer to the newly constructed game object
	static GameObject* CreateMonkey(void);

	static GameObject* CreateCheckpoint();

	static GameObject* CreateGoal();

	// Handle movement logic for the monkey game object
	static void MoveMonkey(void);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	static AEGfxTexture* textureMonkey;
	static AEGfxVertexList* meshQuad;
	static SpriteSource* spriteSourceMonkey;
	static GameObject* gameObjectMonkey;

	static AEGfxTexture* textureGoal;
	static AEGfxVertexList* meshGoal;
	static SpriteSource* spriteSourceGoal;
	static GameObject* gameObjectGoal;

	static AEGfxTexture* textureCheckpoint;
	static AEGfxVertexList* meshCheckpoint;
	static GameObject* gameObjectCheckpoint;
	static SpriteSource* spriteSourceCheckpoint;

	static const float monkeyWalkSpeed;
	static const float monkeyJumpSpeed;
	static const float groundHeight;

	static const Vector2D gravityNormal;
	static const Vector2D gravityNone;
};
/*----------------------------------------------------------------------------*/
