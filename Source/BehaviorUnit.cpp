//------------------------------------------------------------------------------
//
// File Name:	BehaviorUnit.cpp
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
#include "BehaviorUnit.h"
#include "GameObjectManager.h"
#include "GameStateDemo.h"
#include "Tilemap.h"
#include "Trace.h"

//------------------------------------------------------------------------------
enum UnitState { cUnitInvalid, cUnitWaiting, cUnitMoving, cUnitCheckMove, };
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Unit) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorUnit::BehaviorUnit(GameObject& parent, Army::Unit unit, vector<Vector2D> path)
	: base(parent)
{
	base.stateCurr = cUnitInvalid;
	base.stateNext = cUnitMoving;
	base.onInit = BehaviorUnit::Init;
	base.onUpdate = BehaviorUnit::Update;
	base.onExit = BehaviorUnit::Exit;
	base.clone = BehaviorUnit::Clone;
	base.destroy = BehaviorUnit::Destroy;
	unitData = unit;
	hp = unitData.hp;
	this->path = path;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Copy an existing unit behavior component.
// Params:
//  other  = The behavior being copied.
//  parent = The object that owns this behavior.
BehaviorUnit::BehaviorUnit(const Behavior& other, GameObject& parent)
	: base(parent)
{
	base.stateCurr = other.stateCurr;
	base.stateNext = other.stateNext;
	base.onInit = other.onInit;
	base.onUpdate = other.onUpdate;
	base.onExit = other.onExit;
	base.clone = other.clone;
	base.destroy = other.destroy;
	unitData = ((const BehaviorUnit&)other).unitData;
	hp = unitData.hp;
	path = ((const BehaviorUnit&)other).path;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Behavior* BehaviorUnit::Clone(const Behavior& behavior, GameObject& parent)
{
	return (Behavior*) new BehaviorUnit(behavior, parent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorUnit::Destroy(Behavior& behavior)
{
	delete (BehaviorUnit*)&behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorUnit::Init(Behavior& behavior)
{
	BehaviorUnit &self = *((BehaviorUnit*)(&behavior));
	switch (behavior.stateCurr)
	{
	case cUnitMoving:
		behavior.stateCurr = cUnitCheckMove; // Prevent self-detection
		vector<GameObject*> units = GameObjectManager::GetInstance().GetObjectsByName("Unit");
		for (GameObject* obj : units) {
			BehaviorUnit *bu = (BehaviorUnit*)(obj->GetBehavior());
			Transform *t = obj->GetTransform();
			if (!bu || !t) continue; // If missing behavior or transform, skip
			if (bu->base.stateCurr == cUnitMoving) { // If other already moving
				if (bu->GetNextPos() == self.GetNextPos()) {
					behavior.stateCurr = cUnitWaiting; // Skip init
					behavior.stateNext = cUnitWaiting;
					break;
				}
			}
		}
		if (behavior.stateCurr == cUnitCheckMove) {
			behavior.stateCurr = cUnitMoving;
			Vector2D direction = self.GetNextDir();
			direction.X(direction.X() * GameStateDemo::tilemap->getTileWidth());
			direction.Y(direction.Y() * GameStateDemo::tilemap->getTileHeight());
			direction *= (float)(self.unitData.speed);
			direction /= 100;
			behavior.parent.GetPhysics()->SetVelocity(direction);
		}
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorUnit::Update(Behavior& behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	//BehaviorUnit &self = *((BehaviorUnit*)(&behavior));
	switch (behavior.stateCurr)
	{
	case cUnitMoving:
		//if (self.GetScrPos())
		break;
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorUnit::Exit(Behavior& behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

Vector2D BehaviorUnit::GetScrPos()
{
	return base.parent.GetTransform()->GetTranslation();
}

Vector2D BehaviorUnit::GetMapPos()
{
	return GameStateDemo::tilemap->getPosOnMap(GetScrPos());
}

Vector2D BehaviorUnit::GetNextDir()
{
	return path[0];
}

Vector2D BehaviorUnit::GetNextPos()
{
	return GetMapPos()+GetNextDir();
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
