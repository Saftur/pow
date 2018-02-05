//------------------------------------------------------------------------------
//
// File Name:	BehaviorGoal.h
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

#include "GameObject.h"
#include "Behavior.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// An example of the enums to be defined in BehaviorGoal.cpp.
#if 0
enum GoalState
{
	cGoalInvalid,
	cGoalIdle,
};

typedef enum
{
	cGoalSmall,
	cGoalMedium,
	cGoalLarge,

} GoalSize;

typedef enum
{
	cGoalOriginTlc,
	cGoalOriginTrc,
	cGoalOriginBlc,
	cGoalOriginBrc,
	cGoalOriginCount,

} GoalOrigin;
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorGoal.h
class BehaviorGoal : Behavior
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Goal) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorGoal();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Copy an existing asteroid behavior component.
	// Params:
	//  other  = The behavior being copied.
	//  parent = The object that owns this behavior.
	BehaviorGoal(const Behavior& other, GameObject& parent);

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

	float alpha;

	int nextLevel;

	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
