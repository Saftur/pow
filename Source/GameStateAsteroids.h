//------------------------------------------------------------------------------
//
// File Name:	GameStateAsteroids.h
// Author(s):	Doug Schilling (dschilling)
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

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class GameStateAsteroids
{
public:
	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Load the resources associated with the Asteroids game state.
	static void Load();

	// Initialize the memory associated with the Asteroids game state.
	static void Init();

	// Update the Asteroids game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Update(float dt);

	// Shutdown any memory associated with the Asteroids game state.
	static void Shutdown();

	// Unload the resources associated with the Asteroids game state.
	static void Unload();

	// Increase the asteroids score by score value.
	// Params:
	//	 scoreValue = The amount to be added to the game score.
	static void IncreaseScore(unsigned int scoreValue);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	static const int cAsteroidSpawnInitial = 8;
	static const int cAsteroidSpawnMaximum = 20;
	static const int scoreStringLength = 128;

	static AEGfxVertexList*	pMeshSpaceship;
	static AEGfxVertexList*	pMeshBullet;
	static AEGfxVertexList*	pMeshAsteroid;
	static int asteroidScore;
	static int asteroidHighScore;
	static int asteroidSpawnCount;
	static int asteroidWaveCount;

	static char score[scoreStringLength];

	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	// Create meshes for spaceship and bullet
	static void CreateMeshes(void);
	// Free all created meshes.
	static void FreeMeshes(void);

	// Single objects
	static void CreateSpaceship(void);
	static void SpawnAsteroidWave(void);
	static void SpawnAsteroid(void);
	static void UpdateScore(void);

	// Archetypes
	static void CreateBulletArchetype(void);
	static void CreateAsteroidArchetype(void);
	
	static void UpdateCamera(float dt);
};

/*----------------------------------------------------------------------------*/
