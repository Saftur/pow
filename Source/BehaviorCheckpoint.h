//------------------------------------------------------------------------------
//
// File Name:	BehaviorCheckpoint.h
// Author(s):	Mark Culp
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

#include "GameObject.h"
#include "Behavior.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

const float WorldBottom = AEGfxGetWinMaxY();

// An example of the enums to be defined in BehaviorCheckpoint.cpp.
#if 0
enum CheckpointState
{
	cCheckpointInvalid,
	cCheckpointIdle,
};

typedef enum
{
	cCheckpointSmall,
	cCheckpointMedium,
	cCheckpointLarge,

} CheckpointSize;

typedef enum
{
	cCheckpointOriginTlc,
	cCheckpointOriginTrc,
	cCheckpointOriginBlc,
	cCheckpointOriginBrc,
	cCheckpointOriginCount,

} CheckpointOrigin;
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorCheckpoint.h
class BehaviorCheckpoint : Behavior
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Checkpoint) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorCheckpoint(GameObject& parent);

	static void ResetUnconditional();

	static GameObject& GetActiveCheckpoint();

	static void SetPlayer(GameObject& player);

	static void CheckPos();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Copy an existing asteroid behavior component.
	// Params:
	//  other  = The behavior being copied.
	//  parent = The object that owns this behavior.
	BehaviorCheckpoint(const Behavior& other, GameObject& parent);

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

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Inherit the base behavior structure.

	static GameObject* activeCheckpoint;
	static GameObject* player;


	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
