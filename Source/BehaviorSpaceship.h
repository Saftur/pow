//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.h
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

#include "Behavior.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// An example of the enums to be defined in BehaviorSpaceship.cpp.
#if 0
enum ShipState
{
	cSpaceshipInvalid,
	cSpaceshipIdle,
	cSpaceshipThrust,
};
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class BehaviorSpaceship : public Behavior
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Dynamically allocate a new (Spaceship) behavior component.
	BehaviorSpaceship();

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

	// Calculate the rotation of the ship based on input.
	void UpdateRotation();

	// Calculate the velocity of the ship based on its direction.
	// Params:
	//   dt = Change in time (in seconds) since the last game loop.
	void UpdateVelocity(float dt);

	// Update the weapon timer.
	// Params:
	//   dt = Change in time (in seconds) since the last game loop.
	void UpdateWeapon(float dt);

	// Spawn a bullet at the ship's position with velocity based on ship's direction.
	void SpawnBullet();

	// The collision handling function for Spaceships.
	// Params:
	//	 spaceship = The spaceship object.
	//	 other = The object the spaceship is colliding with.
	static void CollisionHandler(GameObject& spaceship, GameObject& other);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	const float spaceshipAcceleration = 150.0f;
	const float spaceshipSpeedMax = 500.0f;
	const float spaceshipTurnRateMax = (float)M_PI / 1.5f;
	const float spaceshipWeaponCooldownTime = 0.25f;
	const float spaceshipWeaponBulletSpeed = 750.0f;
	const float spaceshipDeathDuration = 3.0f;
};

//------------------------------------------------------------------------------
