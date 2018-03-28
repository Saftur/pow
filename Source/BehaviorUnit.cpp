//------------------------------------------------------------------------------
//
// File Name:	BehaviorUnit.cpp
// Author(s):	Arthur Bouvier
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
#include "BehaviorUnit.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states { cUnitIdle, cUnitMove, cUnitCheckMove, cUnitAttack, cUnitSoftChase,
			  cUnitReturn, cUnitGuard, cUnitFollow, cUnitKill, cUnitBuild, cUnitEndbuild };

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
BehaviorUnit::BehaviorUnit() :
		Behavior("BehaviorUnit")
{
	SetCurrentState(cUnitIdle);
	SetNextState(cUnitMove);
}

void BehaviorUnit::Init(BehaviorArmy::UnitData unitData_, vector<Vector2D> path_, Tilemap *tilemap_)
{
	SetNextState(cUnitMove);
	tilemap = tilemap_;
	unitData = unitData_;
	hp = (float)(unitData.hp);
	startPos = GetMapPos();
	path = path_;
	engagedUnit = nullptr;
	attackTimer = 0;
	payTimer = 0;
	abilitySprite = (Sprite*)GetParent()->GetComponent("Sprite", 1);
	abilitySprite->SetFrame(unitData.ability);
	abilityAnimation = (Animation*)GetParent()->GetComponent("Animation");
	vector<AnimationFrame> seq;
	seq.push_back({ unitData.ability + BehaviorArmy::UnitData::NUMABILITIES, 0.333f });
	seq.push_back({ unitData.ability, 0.0f });
	abilityAnimSequence = AnimationSequence(seq, false);

	unitData.army->IncreaseUnits();
}

void BehaviorUnit::AddPos(Vector2D pos)
{
	
}

Vector2D BehaviorUnit::GetScrPos()
{
	if (!(GetParent()->GetComponent("Transform")))
		return { 0, 0 };
	return ((Transform*)(GetParent()->GetComponent("Transform")))->GetTranslation();
}

Vector2D BehaviorUnit::GetMapPos()
{
	if (!(GetParent()->GetComponent("Transform")))
		return { 0, 0 };
	return tilemap->GetPosOnMap(GetScrPos());
}

Vector2D BehaviorUnit::GetNextDir()
{
	if (path.empty())
		return { 0, 0 };
	return path[0];
}

Vector2D BehaviorUnit::GetNextScrPos()
{
	return tilemap->GetPosOnScreen(GetNextPos());
}

Vector2D BehaviorUnit::GetNextPos()
{
	return startPos+GetNextDir();
}

int BehaviorUnit::GetRecycleReturns()
{
	BehaviorArmy::UnitData ud = unitData;
	ud.hp = (int)hp;
	return ud.GetCost();
}

BehaviorArmy::UnitData BehaviorUnit::GetUnitData()
{
	return unitData;
}

vector<Vector2D> BehaviorUnit::GetPath()
{
	return path;
}

void BehaviorUnit::ClearPath()
{
	Vector2D d = path.empty() ? Vector2D(0, 0) : path[0];
	if (!path.empty())
		path.clear();
	if (IsMoving())
		path.push_back(d);
}

void BehaviorUnit::AddToPath(Vector2D pos)
{
	path.push_back(pos);
}

void BehaviorUnit::AddToPath(vector<Vector2D> path_)
{
	for (Vector2D d : path_)
		path.push_back(d);
}

bool BehaviorUnit::IsMoving()
{
	return GetCurrentState() == cUnitMove;
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

BehaviorArmy *BehaviorUnit::GetArmy()
{
	return unitData.army;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* BehaviorUnit::Clone() const
{
	return new BehaviorUnit(*this);
}

void BehaviorUnit::OnDestroy()
{
	unitData.army->DescreaseUnits();
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorUnit::OnEnter()
{
	switch (GetCurrentState())
	{
	case cUnitMove:
		unitData.army->PushFrontLine(GetMapPos());
		if (path.empty() || engagedUnit) {
			SetCurrentState(cUnitIdle);
			SetNextState(cUnitIdle);
			break;
		}
		Vector2D nextPos = GetNextPos();
		while (nextPos.X() < 0 || nextPos.Y() < 0 ||
			nextPos.X() >= tilemap->GetTilemapWidth() ||
			nextPos.Y() >= tilemap->GetTilemapHeight()) {
			path.erase(path.begin());
			nextPos = GetNextPos();
		}
		if (path.empty()) {
			SetCurrentState(cUnitIdle);
			SetNextState(cUnitIdle);
			break;
		}
		SetCurrentState(cUnitCheckMove); // Prevent  self detection
		vector<GameObject*> units = GetParent()->GetObjectManager()->GetObjectsByName("Unit");
		for (GameObject* obj : units) {
			BehaviorUnit *bu = (BehaviorUnit*)(obj->GetComponent("BehaviorUnit"));
			Transform *t = (Transform*)(obj->GetComponent("Transform"));
			if (!bu || !t || bu == this) continue; // If missing behavior or transform, skip
			if (bu->GetCurrentState() == cUnitMove || bu->GetCurrentState() == cUnitIdle/*cUnitDoneMove*/) { // If other already moving
				if (unitData.army->GetSide() != bu->unitData.army->GetSide() && (IsAdjacent(bu) || (bu->GetCurrentState() == cUnitMove && WillBeAdjacent(bu)))) {
					engagedUnit = bu;
					SetCurrentState(cUnitIdle);
					SetNextState(cUnitIdle);
					break;
				}
				if ((GetNextPos() == bu->GetMapPos()/* && GetNextDir() != bu->GetNextDir()*/) || (GetNextPos() == bu->GetNextPos() && bu->GetCurrentState() == cUnitMove)) {
					SetCurrentState(cUnitIdle); // Skip init
					SetNextState(cUnitIdle);
					break;
				}
			}
		}
		if (BehaviorArmy::costType == 0) {
			if (!(unitData.army->TakeFromFunds((unsigned)(unitData.GetCost() / 15 + 0.5f)))) {
				SetCurrentState(cUnitIdle);
				SetNextState(cUnitIdle);
			}
		}
		if (GetCurrentState() == cUnitCheckMove) {
			SetCurrentState(cUnitMove);
			Vector2D direction = GetNextDir();
			direction.X(direction.X() * tilemap->GetTileWidth());
			direction.Y(direction.Y() * tilemap->GetTileHeight());
			direction *= (float)(unitData.speed);
			direction /= 100;
			direction.Y(-direction.Y());
			((Physics*)(GetParent()->GetComponent("Physics")))->SetVelocity(direction);
		}
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorUnit::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (GetParent()->IsDestroyed())
		return;
	/*if (hp <= 0) {
		GetParent()->Destroy();
		abilityOverlay->Destroy();
		return;
	}*/

	switch (GetCurrentState())
	{
	case cUnitMove: {
		Vector2D dir = GetNextDir();
		Vector2D scrPos = GetScrPos();
		Vector2D nextScrPos = GetNextScrPos();
		Vector2D diff = scrPos - nextScrPos;
		if (diff.X() * dir.X() >= 0 && diff.Y() * -dir.Y() >= 0) {
			((Physics*)(GetParent()->GetComponent("Physics")))->SetVelocity({ 0, 0 });
			path.erase(path.begin());
			SetCurrentState(cUnitIdle);//cUnitDoneMove;
			startPos = GetMapPos();
			((Transform*)GetParent()->GetComponent("Transform"))->SetTranslation(tilemap->GetPosOnScreen(GetMapPos()));
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
	case cUnitIdle:
		if (!engagedUnit) {
			vector<GameObject*> units = GetParent()->GetObjectManager()->GetObjectsByName("Unit");
			for (GameObject* obj : units) {
				BehaviorUnit *bu = (BehaviorUnit*)(obj->GetComponent("BehaviorUnit"));
				Transform *t = (Transform*)(obj->GetComponent("Transform"));
				if (!bu || !t || bu == this) continue; // If missing behavior or transform, skip
				if (bu->GetCurrentState() == cUnitMove || bu->GetCurrentState() == cUnitIdle/*cUnitDoneMove*/) { // If other already moving
					if (unitData.army != bu->unitData.army && (IsAdjacent(bu) || (bu->GetCurrentState() == cUnitMove && WillBeAdjacent(bu)))) {
						engagedUnit = bu;
						break;
					}
				}
			}
		}
		if (engagedUnit) {
			BehaviorUnit* engagedBehavior = (BehaviorUnit*)engagedUnit->GetComponent("BehaviorUnit");
			if (!(engagedUnit->IsDestroyed()) && IsAdjacent(engagedBehavior)) {
				if (attackTimer <= 0) {
					if (unitData.army->costType != 0 || unitData.army->TakeFromFunds((unsigned)(unitData.GetCost() / 10 + 0.5f))) {
						//Trace::GetInstance().GetStream() << "Attack!" << std::endl;
						abilityAnimation->PlaySequence(&abilityAnimSequence);
						engagedBehavior->hp -= (float)(unitData.damage) / 4;
						if (engagedBehavior->hp <= 0) {
							engagedUnit->Destroy();
							engagedUnit = nullptr;
						}
						attackTimer = attackCooldown;
					}
				} else attackTimer -= dt;
				break;
			} else engagedUnit = nullptr;
		} else {
			if (BehaviorArmy::costType == 0) {
				if (payTimer <= 0) {
					if (!(unitData.army->TakeFromFunds((unsigned)(unitData.GetCost() / 20 + 0.5f))))
						GetParent()->Destroy();
					payTimer = payCooldown;
				} else payTimer -= dt;
			}
		}
		SetNextState(cUnitMove);
		break;
	}

	// Engage or disengage an enemy unit.
	if (engagedUnit == nullptr)
	{
		engagedUnit = GetUnitWithinRadius();
	}
	else if (engagedUnit->IsDestroyed())
	{
		engagedUnit = nullptr;
	}
}

void BehaviorUnit::OnExit()
{
}

void BehaviorUnit::Load(rapidjson::Value & obj)
{
	UNREFERENCED_PARAMETER(obj);
}

GameObject* BehaviorUnit::GetUnitWithinRadius()
{
	vector<GameObject*> units = GetParent()->GetObjectManager()->GetObjectsByName("Unit");

	for (GameObject* unit : units)
	{
		if (GetScrPos().Distance(((Transform*)unit->GetComponent("Transform"))->GetTranslation()) < engageDistance)
		{
			if (((BehaviorUnit*)unit->GetComponent("BehaviorUnit"))->unitData.army != unitData.army)
			{
				return unit;
			}
		}
	}

	return nullptr;
}

// The collision handling function for Units.
// Params:
//	 stub = The stub object.
//	 other = The object the asteroid is colliding with.
void BehaviorUnit::CollisionHandler(GameObject& stub, GameObject& other)
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
