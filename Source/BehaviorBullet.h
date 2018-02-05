//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.h
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

// An example of the enums to be defined in BehaviorBullet.cpp.
#if 0
enum BulletState
{
	cBulletIdle,
};
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class BehaviorBullet : public Behavior
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new Bullet behavior component.
	BehaviorBullet();

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

	// Decrease the life timer and destroy the bullet. 
	void UpdateLifeTimer(float dt);

	// The collision handling function for bullets.
	// Params:
	//	 bullet = The bullet object.
	//	 other = The object the bullet is colliding with.
	static void CollisionHandler(GameObject& bullet, GameObject& other);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	// Maximum speed of the bullet.
	const float bulletSpeedMax = 500.0f;
	// Maximum lifetime of a bullet (in seconds).
	const float bulletLifeTimeMax = 1.0f;
};

//------------------------------------------------------------------------------
