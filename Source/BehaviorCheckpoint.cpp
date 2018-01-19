//------------------------------------------------------------------------------
//
// File Name:	BehaviorCheckpoint.cpp
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
#include "BehaviorCheckpoint.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states { cCheckpointInvalid, cCheckpointIdle, cCheckpointReset };

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Checkpoint) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorCheckpoint::BehaviorCheckpoint(GameObject& parent)
	: base(parent)
{
	base.stateCurr = cCheckpointInvalid;
	base.stateNext = cCheckpointIdle;
	base.onInit = BehaviorCheckpoint::Init;
	base.onUpdate = BehaviorCheckpoint::Update;
	base.onExit = BehaviorCheckpoint::Exit;
	base.clone = BehaviorCheckpoint::Clone;
	base.destroy = BehaviorCheckpoint::Destroy;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Copy an existing asteroid behavior component.
// Params:
//  other  = The behavior being copied.
//  parent = The object that owns this behavior.
BehaviorCheckpoint::BehaviorCheckpoint(const Behavior& other, GameObject& parent)
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
Behavior* BehaviorCheckpoint::Clone(const Behavior& behavior, GameObject& parent)
{
	return (Behavior*) new BehaviorCheckpoint(behavior, parent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorCheckpoint::Destroy(Behavior& behavior)
{
	delete (BehaviorCheckpoint*)&behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorCheckpoint::Init(Behavior& behavior)
{
	switch (behavior.stateCurr)
	{
	case cCheckpointInvalid:
		break;
	case cCheckpointIdle:
		behavior.parent.GetCollider()->SetCollisionHandler(CollisionHandler);
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorCheckpoint::Update(Behavior& behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	switch (behavior.stateCurr)
	{
	case cCheckpointInvalid:
		break;
	case cCheckpointIdle:
		break;
	case cCheckpointReset:
		GameObjectManager::GetInstance().GetObjectByName("Player")->GetTransform()->SetTranslation(activeCheckpoint->GetTransform()->GetTranslation());
		behavior.stateNext = cCheckpointIdle;
		break;
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorCheckpoint::Exit(Behavior& behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// The collision handling function for Checkpoints.
// Params:
//	 asteroid = The asteroid object.
//	 other = The object the asteroid is colliding with.
void BehaviorCheckpoint::CollisionHandler(GameObject& checkpoint, GameObject& other)
{
	if (other.IsNamed("Player"))
	{
		BehaviorCheckpoint::activeCheckpoint = &checkpoint;
	}
}

void BehaviorCheckpoint::CheckPos(GameObject& player)
{
	if (player.GetTransform()->GetTranslation().Y() < AEGfxGetWinMaxY())
	{
		activeCheckpoint->GetBehavior()->stateNext = cCheckpointReset;
	}
}

void BehaviorCheckpoint::ResetUnconditional()
{
	activeCheckpoint->GetBehavior()->stateNext = cCheckpointReset;
}

GameObject& BehaviorCheckpoint::GetActiveCheckpoint()
{
	return *activeCheckpoint;
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

GameObject* BehaviorCheckpoint::activeCheckpoint = nullptr;

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
