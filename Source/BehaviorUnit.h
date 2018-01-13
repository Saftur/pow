//------------------------------------------------------------------------------
//
// File Name:	BehaviorUnit.h
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
#include "Army.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorUnit.h
class BehaviorUnit
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Unit) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorUnit(GameObject& parent, Army::Unit unit, vector<Vector2D> path, GameObject* ability);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Copy an existing unit behavior component.
	// Params:
	//  other  = The behavior being copied.
	//  parent = The object that owns this behavior.
	BehaviorUnit(const Behavior& other, GameObject& parent);

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	//   parent = A reference to the parent object (the object that owns this component).
	// Returns:
	//   A pointer to an advanced behavior.
	static Behavior* Clone(const Behavior& behavior, GameObject& parent);

	// Destroy an advanced behavior.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	static void Destroy(Behavior& behavior);

	// Initialize the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	static void Init(Behavior& behavior);

	// Update the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Update(Behavior& behavior, float dt);

	// Exit the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	//	 dt = Change in time (in seconds) since the last game loop.
	static void Exit(Behavior& behavior);

	Vector2D GetScrPos();
	Vector2D GetMapPos();
	Vector2D GetNextDir();
	Vector2D GetNextPos();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Inherit the base behavior structure.
	Behavior	base;

	// Add unit-specific behavior variables.
	Army::Unit unitData;
	int hp;
	vector<Vector2D> path;
	GameObject* abilitySprite;

	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------

};

//------------------------------------------------------------------------------
