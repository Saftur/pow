//------------------------------------------------------------------------------
//
// File Name:	BehaviorStub.h
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

// An example of the enums to be defined in BehaviorStub.cpp.
#if 0
enum StubState
{
	cStubInvalid,
	cStubIdle,
};

typedef enum
{
	cStubSmall,
	cStubMedium,
	cStubLarge,

} StubSize;

typedef enum
{
	cStubOriginTlc,
	cStubOriginTrc,
	cStubOriginBlc,
	cStubOriginBrc,
	cStubOriginCount,

} StubOrigin;
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorStub.h
class BehaviorStub
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Stub) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorStub(GameObject& parent);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Copy an existing asteroid behavior component.
	// Params:
	//  other  = The behavior being copied.
	//  parent = The object that owns this behavior.
	BehaviorStub(const Behavior& other, GameObject& parent);

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

	// The collision handling function for Stubs.
	// Params:
	//	 stub = The stub object.
	//	 other = The object the asteroid is colliding with.
	static void CollisionHandler(GameObject& stub, GameObject& other);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Inherit the base behavior structure.
	Behavior	base;

	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
