//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//----------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorButtonStub.h"
#include "GameObject.h"
#include "Random.h"
#include "AEEngine.h"
#include "GameObjectManager.h"

typedef enum
{
	cButtonInvalid,
	cButtonIdle,
} ButtonState;

// Allocate a new (Asteroid) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorButtonStub::BehaviorButtonStub(GameObject& parent) : base(parent) {
	//When about a million errors show up here, go to Behavior.h and add "friend class (Class Name);" to it.
	base.clone = Clone;
	base.destroy = Destroy;
	base.onExit = Exit;
	base.onInit = Init;
	base.onUpdate = Update;
	base.parent = parent;
	base.stateCurr = cButtonInvalid;
	base.stateNext = cButtonIdle;
}

// Copy an existing asteroid behavior component.
// Params:
//  other  = The behavior being copied.
//  parent = The object that owns this behavior.
BehaviorButtonStub::BehaviorButtonStub(const Behavior& other, GameObject& parent) : base(other, parent) {
	base.stateCurr = other.stateCurr;
	base.stateNext = other.stateNext;
	base.onInit = other.onInit;
	base.onUpdate = other.onUpdate;
	base.onExit = other.onExit;
	base.clone = other.clone;
	base.destroy = other.destroy;

	base.stateCurr = cButtonInvalid;
	base.stateNext = cButtonIdle;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Behavior* BehaviorButtonStub::Clone(const Behavior& behavior, GameObject& parent) {
	return (Behavior*)(new BehaviorButtonStub(behavior, parent));
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorButtonStub::Destroy(Behavior& behavior) {
	delete ((BehaviorButtonStub*)&behavior);
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorButtonStub::Init(Behavior& behavior) {
	if (behavior.stateCurr == cButtonIdle) {
		behavior.parent.GetCollider()->SetCollisionHandler(CollisionHandler);
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorButtonStub::Update(Behavior& behavior, float dt) {
	UNREFERENCED_PARAMETER(behavior);
	UNREFERENCED_PARAMETER(dt);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorButtonStub::Exit(Behavior& behavior) {
	UNREFERENCED_PARAMETER(behavior);
}

// The collision handling function for Asteroids.
// Params:
//	 asteroid = The asteroid object.
//	 other = The object the asteroid is colliding with.
void BehaviorButtonStub::CollisionHandler(GameObject& button, GameObject& other) {
	UNREFERENCED_PARAMETER(button);
	if (other.IsNamed("ThisIsAFakeObjectSoThisNameShouldntExistElsewhere")) {
		//Place all code here that you want triggered when someone presses this button type.
	}
}
