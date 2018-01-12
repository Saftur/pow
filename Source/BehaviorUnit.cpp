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
enum UnitState { cUnitInvalid, cUnitWaiting, cUnitMoving, cUnitCheckMove, cUnitAttacking };
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
	hp = (float)(unitData.hp);
	startPos = GetMapPos();
	this->path = path;
	target = nullptr;
	attackTimer = 0;
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
	hp = (float)(unitData.hp);
	startPos = GetMapPos();
	path = ((const BehaviorUnit&)other).path;
	target = ((const BehaviorUnit&)other).target;
	attackTimer = 0;
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
		Vector2D nextPos = self.GetNextPos();
		while (nextPos.X() < 0 || nextPos.Y() < 0 ||
			nextPos.X() >= GameStateDemo::tilemap->getTilemapWidth() ||
			nextPos.Y() >= GameStateDemo::tilemap->getTilemapHeight()) {
			self.path.erase(self.path.begin());
			nextPos = self.GetNextPos();
		}
		if (self.path.empty()) {
			behavior.stateCurr = cUnitWaiting;
			behavior.stateNext = cUnitWaiting;
			break;
		}
		behavior.stateCurr = cUnitCheckMove; // Prevent self-detection
		vector<GameObject*> units = GameObjectManager::GetInstance().GetObjectsByName("Unit");
		for (GameObject* obj : units) {
			BehaviorUnit *bu = (BehaviorUnit*)(obj->GetBehavior());
			Transform *t = obj->GetTransform();
			if (!bu || !t) continue; // If missing behavior or transform, skip
			if (bu->base.stateCurr == cUnitMoving || bu->base.stateCurr == cUnitWaiting/*cUnitDoneMove*/) { // If other already moving
				Vector2D sp = self.GetMapPos();
				Vector2D snd = self.GetNextDir();
				Vector2D snp = self.GetNextPos();
				Vector2D op = bu->GetMapPos();
				Vector2D ond = bu->GetNextDir();
				Vector2D onp = bu->GetNextPos();
				if (self.unitData.army != bu->unitData.army && (self.IsAdjacent(bu) || (bu->base.stateCurr == cUnitMoving && self.WillBeAdjacent(bu)))) {
					self.target = bu;
					behavior.stateCurr = cUnitWaiting;
					behavior.stateNext = cUnitWaiting;
					break;
				}
				if ((self.GetNextPos() == bu->GetMapPos()/* && self.GetNextDir() != bu->GetNextDir()*/) || self.GetNextPos() == bu->GetNextPos()) {
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
			direction.Y(-direction.Y());
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

	BehaviorUnit &self = *((BehaviorUnit*)(&behavior));
	if (self.hp <= 0) {
		self.base.parent.Destroy();
		return;
	}

	switch (behavior.stateCurr)
	{
	case cUnitMoving: {
		Vector2D dir = self.GetNextDir();
		Vector2D scrPos = self.GetScrPos();
		Vector2D nextScrPos = self.GetNextScrPos();
		Vector2D diff = scrPos - nextScrPos;
		if (diff.X() * dir.X() >= 0 && diff.Y() * -dir.Y() >= 0) {
			behavior.parent.GetPhysics()->SetVelocity({ 0, 0 });
			self.path.erase(self.path.begin());
			behavior.stateCurr = cUnitWaiting;//cUnitDoneMove;
			self.startPos = self.GetMapPos();
			//Init(behavior);
		}
		/*if (dir.X() != 0) {
			if (scrPos.X()*dir.X() >= nextScrPos.X()*dir.X()) {
				scrPos.X(nextScrPos.X());
			}
		}
		if (dir.Y() != 0) {
			if (scrPos.Y()*dir.Y() >= nextScrPos.Y()*dir.Y()) {
				scrPos.Y(nextScrPos.Y());
			}
		}*/
	}
		break;
	case cUnitWaiting:
		if (self.target) {
			if (self.IsAdjacent(self.target) && !(self.target->base.parent.IsDestroyed())) {
				if (self.attackTimer <= 0) {
					//Trace::GetInstance().GetStream() << "Attack!" << std::endl;
					self.target->hp -= (float)(self.unitData.damage) / 4;
					if (self.target->hp <= 0) self.target = nullptr;
					self.attackTimer = self.attackCooldown;
				} else self.attackTimer -= dt;
				break;
			} else self.target = nullptr;
		}
		behavior.stateNext = cUnitMoving;
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
	if (!(base.parent.GetTransform()))
		return { 0, 0 };
	return base.parent.GetTransform()->GetTranslation();
}

Vector2D BehaviorUnit::GetMapPos()
{
	if (!(base.parent.GetTransform()))
		return { 0, 0 };
	return GameStateDemo::tilemap->getPosOnMap(GetScrPos());
}

Vector2D BehaviorUnit::GetNextDir()
{
	if (path.empty())
		return { 0, 0 };
	return path[0];
}

Vector2D BehaviorUnit::GetNextScrPos()
{
	return GameStateDemo::tilemap->getPosOnScreen(GetNextPos());
}

Vector2D BehaviorUnit::GetNextPos()
{
	return startPos+GetNextDir();
}

bool BehaviorUnit::IsAdjacent(BehaviorUnit * other)
{
	Vector2D diff = GetMapPos() - other->GetMapPos();
	return diff.Magnitude() <= 1;
}

bool BehaviorUnit::WillBeAdjacent(BehaviorUnit * other)
{
	Vector2D diff = GetMapPos() - other->GetNextPos();
	return diff.Magnitude() <= 1;
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
