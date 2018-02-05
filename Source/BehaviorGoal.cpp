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
#include "Collider.h"
#include "Transform.h"
#include "Sprite.h"
#include "Animation.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states {cGoalIdle, cGoalAchieved};

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
BehaviorGoal::BehaviorGoal()
{
	SetNextState(cGoalIdle);
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
Component* BehaviorGoal::Clone() const
{
	return new BehaviorGoal(*this);
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorGoal::OnEnter()
{
	switch (GetCurrentState())
	{
	case cGoalIdle:
		((Collider*)GetParent()->GetComponent("Collider"))->SetCollisionHandler(CollisionHandler);
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorGoal::OnUpdate(float dt)
{
	switch (GetCurrentState())
	{
	case cGoalIdle:
		break;
	case cGoalAchieved:
		((Transform*)GetParent()->GetComponent("Transform"))->SetScale(((Transform*)GetParent()->GetComponent("Transform"))->GetScale() + Vector2D(5.0f * dt, 5.0f * dt));
		((Sprite*)GetParent()->GetComponent("Sprite"))->AdjustAlpha(-0.5f * dt);
		alpha -= 0.5f * dt;

		if (alpha <= 0.0f)
		{
			GameStateManager::GetInstance().SetNextState(nextLevel);
		}

		break;
	}
}

// The collision handling function for Goals.
// Params:
//	 goal = The goal object.
//	 other = The object the asteroid is colliding with.
void BehaviorGoal::CollisionHandler(GameObject& goal, GameObject& other)
{
	if (other.IsNamed("Player") && ((Behavior*)goal.GetComponent("Behavior"))->GetCurrentState() != cGoalAchieved)
	{
		((Behavior*)goal.GetComponent("Behavior"))->SetNextState(cGoalAchieved);
		((Animation*)goal.GetComponent("Animation"))->TogglePause();
	}
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
