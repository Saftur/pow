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

#include "GameState.h"

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

class GameStateAsteroids : public GameState
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create an instance of Asteroids
	GameStateAsteroids();

	// Load the resources associated with the Asteroids game state.
	void Load();

	// Initialize the memory associated with the Asteroids game state.
	void Init();

	// Update the Asteroids game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	// Shutdown any memory associated with the Asteroids game state.
	void Shutdown();

	// Unload the resources associated with the Asteroids game state.
	void Unload();

	// Increase the asteroids score by score value.
	// Params:
	//	 scoreValue = The amount to be added to the game score.
	static void IncreaseScore(unsigned int scoreValue);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	const int cAsteroidSpawnInitial = 8;
	const int cAsteroidSpawnMaximum = 20;
	static const int scoreStringLength = 128;

	AEGfxVertexList*	pMeshSpaceship;
	AEGfxVertexList*	pMeshBullet;
	AEGfxVertexList*	pMeshAsteroid;
	static int asteroidScore;
	static int asteroidHighScore;
	int asteroidSpawnCount;
	static int asteroidWaveCount;

	static char score[scoreStringLength];

	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	// Create meshes for spaceship and bullet
	void CreateMeshes(void);
	// Free all created meshes.
	void FreeMeshes(void);

	// Single objects
	void CreateSpaceship(void);
	void SpawnAsteroidWave(void);
	void SpawnAsteroid(void);
	static void UpdateScore(void);

	// Archetypes
	void CreateBulletArchetype(void);
	void CreateAsteroidArchetype(void);
};

/*----------------------------------------------------------------------------*/
