//------------------------------------------------------------------------------
//
// File Name:	BehaviorStub.cpp
// Author(s):	Mark Culp
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Random.h"
#include "AEEngine.h"
#include "Teleporter.h"
#include "GameStateLevel1.h"
#include "BehaviorStub.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states {cStubInvalid, cStubIdle};

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Stub) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorStub::BehaviorStub(GameObject& parent)
	: base(parent)
{
	base.stateCurr = cStubInvalid;
	base.stateNext = cStubIdle;
	base.onInit = BehaviorStub::Init;
	base.onUpdate = BehaviorStub::Update;
	base.onExit = BehaviorStub::Exit;
	base.clone = BehaviorStub::Clone;
	base.destroy = BehaviorStub::Destroy;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Copy an existing asteroid behavior component.
// Params:
//  other  = The behavior being copied.
//  parent = The object that owns this behavior.
BehaviorStub::BehaviorStub(const Behavior& other, GameObject& parent)
	: base(parent)
{
	base.stateCurr = other.stateCurr;
	base.stateNext = other.stateNext;
	base.onInit = other.onInit;
	base.onUpdate = other.onUpdate;
	base.onExit = other.onExit;
	base.clone = other.clone;
	base.destroy = other.destroy;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Behavior* BehaviorStub::Clone(const Behavior& behavior, GameObject& parent)
{
	return (Behavior*) new BehaviorStub(behavior, parent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorStub::Destroy(Behavior& behavior)
{
	delete (BehaviorStub*)&behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorStub::Init(Behavior& behavior)
{
	switch (behavior.stateCurr)
	{
	case cStubInvalid:
		break;
	case cStubIdle:
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorStub::Update(Behavior& behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	switch (behavior.stateCurr)
	{
	case cStubInvalid:
		break;
	case cStubIdle:
		break;
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorStub::Exit(Behavior& behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// The collision handling function for Stubs.
// Params:
//	 stub = The stub object.
//	 other = The object the asteroid is colliding with.
void BehaviorStub::CollisionHandler(GameObject& stub, GameObject& other)
{
	UNREFERENCED_PARAMETER(stub);
	UNREFERENCED_PARAMETER(other);
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
