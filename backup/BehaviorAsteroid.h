//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.h
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

#include "Behavior.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// An example of the enums to be defined in BehaviorBullet.cpp.
#if 0
enum AsteroidState
{
	cAsteroidIdle,
};

typedef enum
{
	cAsteroidSmall,
	cAsteroidMedium,
	cAsteroidLarge,

} AsteroidSize;

typedef enum
{
	cAsteroidOriginTlc,
	cAsteroidOriginTrc,
	cAsteroidOriginBlc,
	cAsteroidOriginBrc,
	cAsteroidOriginCount,

} AsteroidOrigin;
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorAsteroid.h
class BehaviorAsteroid : public Behavior
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Asteroid) behavior component.
	BehaviorAsteroid();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a dynamically allocated clone of the advanced behavior.
	Component* Clone() const;

	// Initialize the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	void OnEnter();

	// Update the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void OnUpdate(float dt);

	// The collision handling function for Asteroids.
	// Params:
	//	 asteroid = The asteroid object.
	//	 other = The object the asteroid is colliding with.
	static void CollisionHandler(GameObject& asteroid, GameObject& other);

	// Set position of asteroid based on origin
	void SetPosition();

	// Set velocity based on size
	void SetVelocity();

	// Generate new asteroids based off this asteroid
	void SpawnNewAsteroids();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Add asteroid-specific behavior variables.
	int	size;
	int	origin;

	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	const float asteroidSpeedMin = 50.0f;
	const float asteroidSpeedMax = 100.0f;
};

//------------------------------------------------------------------------------
