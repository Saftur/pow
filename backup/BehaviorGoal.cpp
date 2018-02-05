//------------------------------------------------------------------------------
//
// File Name:	BehaviorGoal.cpp
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
#include "BehaviorGoal.h"
#include "GameObjectManager.h"
#include "GameStateManager.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states {cGoalInvalid, cGoalIdle, cGoalAchieved};

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Goal) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorGoal::BehaviorGoal(GameObject& parent, int theNextLevel)
	: base(parent)
{
	base.stateCurr = cGoalInvalid;
	base.stateNext = cGoalIdle;
	base.onInit = BehaviorGoal::Init;
	base.onUpdate = BehaviorGoal::Update;
	base.onExit = BehaviorGoal::Exit;
	base.clone = BehaviorGoal::Clone;
	base.destroy = BehaviorGoal::Destroy;
	alpha = 1.0f;
	nextLevel = theNextLevel;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Copy an existing asteroid behavior component.
// Params:
//  other  = The behavior being copied.
//  parent = The object that owns this behavior.
BehaviorGoal::BehaviorGoal(const Behavior& other, GameObject& parent)
	: base(parent)
{
	base.stateCurr = other.stateCurr;
	base.stateNext = other.stateNext;
	base.onInit = other.onInit;
	base.onUpdate = other.onUpdate;
	base.onExit = other.onExit;
	base.clone = other.clone;
	base.destroy = other.destroy;
	alpha = ((BehaviorGoal&)other).alpha;
	nextLevel = ((BehaviorGoal&)other).nextLevel;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Behavior* BehaviorGoal::Clone(const Behavior& behavior, GameObject& parent)
{
	return (Behavior*) new BehaviorGoal(behavior, parent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorGoal::Destroy(Behavior& behavior)
{
	delete (BehaviorGoal*)&behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorGoal::Init(Behavior& behavior)
{
	switch (behavior.stateCurr)
	{
	case cGoalInvalid:
		break;
	case cGoalIdle:
		behavior.parent.GetCollider()->SetCollisionHandler(CollisionHandler);
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorGoal::Update(Behavior& behavior, float dt)
{
	switch (behavior.stateCurr)
	{
	case cGoalInvalid:
		break;
	case cGoalIdle:
		break;
	case cGoalAchieved:
		behavior.parent.GetTransform()->SetScale(behavior.parent.GetTransform()->GetScale() + Vector2D(5.0f * dt, 5.0f * dt));
		behavior.parent.GetSprite()->AdjustAlpha(-0.5f * dt);
		((BehaviorGoal&)behavior).alpha -= 0.5f * dt;

		if (((BehaviorGoal&)behavior).alpha <= 0.0f)
		{
			GameStateManager::GetInstance().SetNextState(((BehaviorGoal&)behavior).nextLevel);
		}

		break;
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorGoal::Exit(Behavior& behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// The collision handling function for Goals.
// Params:
//	 goal = The goal object.
//	 other = The object the asteroid is colliding with.
void BehaviorGoal::CollisionHandler(GameObject& goal, GameObject& other)
{
	if (other.IsNamed("Player") && goal.GetBehavior()->stateCurr != cGoalAchieved)
	{
		goal.GetBehavior()->stateNext = cGoalAchieved;
		goal.GetAnimation()->TogglePause();
	}
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
