//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel2.h
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class SpriteSource SpriteSource;
typedef class Sprite Sprite;
typedef class Animation Animation;
typedef class AnimationSequence AnimationSequence;
typedef class GameObject GameObject;
typedef struct AnimationFrame AnimationFrame;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
class GameStateLevel2
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

	// Load the resources associated with the Level2 game state.
	static void Load();

	// Initialize the memory associated with the Level2 game state.
	static void Init();

	// Update the Level2 game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Update(float dt);

	// Shutdown any memory associated with the Level2 game state.
	static void Shutdown();

	// Unload the resources associated with the Level2 game state.
	static void Unload();

private:
	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	static GameObject* CreateSpaceship(void);
	static void MoveSpaceship(void);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	static int numLives;
	static AEGfxVertexList* meshTriangle;
	static GameObject* gameObjectShip;
	static const float spaceshipSpeed;
};
/*----------------------------------------------------------------------------*/
