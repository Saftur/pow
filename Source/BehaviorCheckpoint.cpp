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
#include "GameStateManager.h"
#include "GameStateTable.h"
#include "Behavior.h"
#include "Transform.h"
#include "Collider.h"
#include "BehaviorPlayer.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states { cCheckpointIdle, cCheckpointReset };

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
{
	SetNextState(cCheckpointIdle);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* BehaviorCheckpoint::Clone() const
{
	return new BehaviorCheckpoint(*this);
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorCheckpoint::OnEnter()
{
	switch (GetCurrentState())
	{
	case cCheckpointIdle:
		((Collider*)GetParent()->GetComponent("Collider"))->SetCollisionHandler(CollisionHandler);
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorCheckpoint::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	switch (GetCurrentState())
	{
	case cCheckpointIdle:
		break;
	case cCheckpointReset:
		((Transform*)player->GetComponent("Transform"))->SetTranslation(((Transform*)activeCheckpoint->GetComponent("Transform"))->GetTranslation());
		((BehaviorPlayer*)player->GetComponent("Behavior"))->ResetSpeedModifier();
		SetNextState(cCheckpointIdle);
		break;
	}
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

void BehaviorCheckpoint::SetPlayer(GameObject& player_) {
	player = &player_;
}

void BehaviorCheckpoint::CheckPos()
{
	if (((Transform*)player->GetComponent("Transform"))->GetTranslation().Y() < AEGfxGetWinMinY())
	{
		ResetUnconditional();
	}
}

void BehaviorCheckpoint::ResetUnconditional()
{
	if (activeCheckpoint)
		((BehaviorCheckpoint*)activeCheckpoint->GetComponent("Behavior"))->SetNextState(cCheckpointReset);
	else GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);
}

GameObject& BehaviorCheckpoint::GetActiveCheckpoint()
{
	return *activeCheckpoint;
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

GameObject* BehaviorCheckpoint::activeCheckpoint = nullptr;
GameObject* BehaviorCheckpoint::player = nullptr;

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
