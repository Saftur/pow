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
#include "PlatformManager.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class BehaviorPlayer : public Behavior
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Dynamically allocate a new (Spaceship) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorPlayer();

	void ResetSpeedModifier();

private:
	// The collision handling function for bullets, except it handles collisions as though it was a spaceship as a meme.
	// Params:
	//	 bullet = The bullet object.
	//	 other = The object the bullet is colliding with.
	static void CollisionHandler(GameObject& bullet, GameObject& other);

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	//   parent = A reference to the parent object (the object that owns this component).
	// Returns:
	//   A pointer to an advanced behavior.
	Component* Clone() const;

	// Initialize the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	void OnEnter();

	// Update the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void OnUpdate(float dt);

	// Calculate the velocity of the ship based on its direction.
	// Params:
	//   behavior = Pointer to the behavior component.
	//   dt = Change in time (in seconds) since the last game loop.
	void UpdateVelocity(float dt);

	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

	static float playerSpeedModifier;
	static const float playerSpeedMax;

	static const float maxPlayerTime;
	static const float playerJumpSpeedMax;
	static float playerGravity;
	static bool jumping;
	static bool wasJumping;
	static bool inAir;
	static PlatformManager::Platform* wasGrounded;
	static float touchTime;
};

//------------------------------------------------------------------------------
