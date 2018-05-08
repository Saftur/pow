//------------------------------------------------------------------------------
//
// File Name:	BehaviorTestBox.cpp
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
#include "BehaviorTestBox.h"
#include "GameObjectManager.h"
#include "Sprite.h"
#include "Transform.h"
#include "Grid.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states { cNodeOpen, cNodeClosed, cTestBoxIdle };

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (TestBox) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorTestBox::BehaviorTestBox() :
	Behavior("BehaviorTestBox")
{
	SetCurrentState(cBehaviorInvalid);
	SetNextState(cTestBoxIdle);
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* BehaviorTestBox::Clone() const
{
	return new BehaviorTestBox(*this);
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorTestBox::OnEnter()
{
	switch (GetCurrentState())
	{
	case cTestBoxIdle:
		break;
	}
}

void BehaviorTestBox::SetBoundPos(Vector2D pos)
{
	gridPos = pos;
}

Vector2D BehaviorTestBox::GetBoundPos()
{
	return gridPos;
}

void BehaviorTestBox::SetNodeState(bool state)
{
	SetNextState(state);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorTestBox::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	switch (GetCurrentState())
	{
	case cTestBoxIdle:
		
		SetNextState(cNodeOpen);
		break;
	case cNodeOpen:
		GetParent()->GetComponent<Transform>()->SetTranslation(Grid::GetInstance().ConvertToWorldPoint(gridPos));
		GetParent()->GetComponent<Sprite>()->SetModulateColor({ 0, 1, 0, 1 });
		break;
	case cNodeClosed:
		GetParent()->GetComponent<Transform>()->SetTranslation(Grid::GetInstance().ConvertToWorldPoint(gridPos));
		GetParent()->GetComponent<Sprite>()->SetModulateColor({ 1, 0, 0, 1 });
		break;
	}
}

void BehaviorTestBox::OnExit()
{
	switch (GetCurrentState()) {
	case cTestBoxIdle:
		break;
	}
}

// The collision handling function for TestBoxs.
// Params:
//	 stub = The stub object.
//	 other = The object the asteroid is colliding with.
void BehaviorTestBox::CollisionHandler(GameObject& stub, GameObject& other)
{
	UNREFERENCED_PARAMETER(stub);
	UNREFERENCED_PARAMETER(other);
}

void BehaviorTestBox::Load(rapidjson::Value& obj)
{

}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
