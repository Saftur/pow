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
#include <algorithm>
#include "AEEngine.h"
#include "Teleporter.h"
#include "BehaviorUnit.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Pathfinder.h"
#include "GridManager.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

vector<GameObject*> BehaviorUnit::allUnits;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

BehaviorUnit::BaseStats BehaviorUnit::defaultStats = 
{
100,		// Max HP.
0,			// Defense
5,			// Detection range.
8,			// Disengagement range.
3,			// Inventory size.
1.0f		// Movement speed.
};

BehaviorUnit::Weapon BehaviorUnit::Weapons[cNumWeapons];
BehaviorUnit::Equipment BehaviorUnit::Equips[cNumEquips];

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Unit) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorUnit::BehaviorUnit() :
	Behavior("BehaviorUnit")
{
	SetCurrentState(cBehaviorInvalid);
	SetNextState(cUnitError);
}

void BehaviorUnit::Init(Traits& theTraits, BehaviorArmy* theArmy)
{
	army = theArmy;

	// Build the static arrays if they haven't already been built.
	BuildArrays();

	// Initialize this unit's traits.
	traits = theTraits;

	stats.attackRange = Weapons[traits.weapon].range;
	stats.maxHP = (int)(defaultStats.maxHP + 0.05f * traits.strength + 0.15f * traits.defense);
	stats.currHP = stats.maxHP;
	stats.inventorySize = (int)(defaultStats.inventorySize + 0.5f * traits.strength + 0.5f * traits.agility);
	stats.detectRange = defaultStats.detectRange;
	stats.defense = (int)(defaultStats.defense + 0.05f * traits.defense);

	// If there's armor, apply an HP boost. If there's an empty slot, give an inventory size boost.
	if (Equips[traits.item1].name == Equips[cEquipArmor].name)
	{
		stats.currHP += 50;
	}
	else if (Equips[traits.item1].name == Equips[cEquipNone].name)
	{
		stats.inventorySize += 2;
	}

	if (Equips[traits.item2].name == Equips[cEquipArmor].name)
	{
		stats.currHP += 50;
	}
	else if (Equips[traits.item2].name == Equips[cEquipNone].name)
	{
		stats.inventorySize += 2;
	}

	
}

void BehaviorUnit::SetPath(std::vector<Node*> newPath)
{
	path = newPath;
	currMoveTarget = GridManager::GetInstance().GetNode(gridPos);
	
	if (path.size() > 0)
		targetPos = newPath[0]->gridPos();
	else
		targetPos = gridPos;

	SetNextState(cUnitMove);
}

void BehaviorUnit::BuildArrays()
{
	// Prevent duplicate initialization.
	if (Weapons[0].name == "Drillsaw")
		return;

	// Build weapon array.
	Weapons[cWeaponDrillsaw] = { "Drillsaw", cGroupMelee, 0.05f, 1, 10, GetParent()->GetGameObjectManager()->GetArchetype("ProjectileInvisible") };
	Weapons[cWeaponHandcannon] = { "Handcannon", cGroupRanged, 0.2f, 3, 20, GetParent()->GetGameObjectManager()->GetArchetype("ProjectileLaser") };
	Weapons[cWeaponBeamRifle] = { "Beam Rifle", cGroupLongRanged, 0.5f, 5, 30, GetParent()->GetGameObjectManager()->GetArchetype("ProjectileLaser") };

	// Build equipment array.
	Equips[cEquipNone] = { "null", 0, UseNone, 0.0f, 0.0f };
	Equips[cEquipArmor] = { "Diamondium Armor", -1, UseArmor, 0.0f, 0.0f };
	Equips[cEquipStrobebang] = { "Strobebang", 5, UseStrobebang, 1.0f, 30.0f };
	Equips[cEquipEMP] = { "EMP", 5, UseEMP, 1.0f, 30.0f };
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

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorUnit::OnEnter()
{
	gridPos = GridManager::GetInstance().ConvertToGridPoint(GetParent()->GetComponent<Transform>()->GetTranslation());
	GridManager::GetInstance().SetNode(gridPos, false);

	switch (GetCurrentState())
	{
	case cUnitError:
		GridManager::GetInstance().GetNode(gridPos)->open = false;
		allUnits.push_back(GetParent());
		break;
	case cUnitIdle:
		break;
	case cUnitMove:
		break;
	case cUnitAttack:
		break;
	case cUnitSoftChase:
		break;
	case cUnitReturn:
		break;
	case cUnitGuard:
		break;
	case cUnitFollow:
		break;
	case cUnitBuild:
		break;
	case cUnitEndBuild:
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

	switch (GetCurrentState())
	{
	case cUnitIdle:
		// Check for enemies.
		CheckAttack();
		break;
	case cUnitMove:

		if (lastMoveTarget.Distance(GetParent()->GetComponent<Transform>()->GetTranslation()) >= GridManager::GetInstance().tileWidth / 2)
			GridManager::GetInstance().GetNode(GridManager::GetInstance().ConvertToGridPoint(lastMoveTarget))->open = true;

		// Are we there yet?
		if (Vector2D::AlmostEquals(GetParent()->GetComponent<Transform>()->GetTranslation(), GridManager::GetInstance().ConvertToWorldPoint(currMoveTarget->gridPos()), 2.5f))
		{
			if (currMoveTarget)
				currMoveTarget->open = true;

			// Do we have more movements to make?
			if (path.empty() || !currMoveTarget->open)
			{
				//if (!currMoveTarget->open)
					//UpdatePath();

				currMoveTarget = nullptr;
				GetParent()->GetComponent<Physics>()->SetVelocity(Vector2D(0, 0));
				SetNextState(cUnitIdle);
				return;
			}

			GridManager::GetInstance().GetNode(gridPos)->open = true;
			
			// Update our target.
			gridPos = currMoveTarget->gridPos();
			GetParent()->GetComponent<Transform>()->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(gridPos));
			
			lastMoveTarget = GridManager::GetInstance().ConvertToWorldPoint(currMoveTarget);

			currMoveTarget = *(path.end() - 1);

			if (currMoveTarget && !currMoveTarget->open)
			{
				currMoveTarget = nullptr;
				GetParent()->GetComponent<Physics>()->SetVelocity(Vector2D(0, 0));
				SetNextState(cUnitIdle);

				UpdatePath();
				return;
			}

			currMoveTarget->open = false;

			path.pop_back();
		}

		// Update velocity.
		CalculateVelocity();
		break;
	case cUnitAttack:
		// Can we attack our target?
		if (CheckAttack())
		{

		}
		else
		{
			// Return to the previous state.
			SetNextState(prevState);
		}
		break;
	case cUnitSoftChase:
		break;
	case cUnitReturn:
		break;
	case cUnitGuard:
		break;
	case cUnitFollow:
		break;
	case cUnitBuild:
		break;
	case cUnitEndBuild:
		break;
	}
}

void BehaviorUnit::OnExit()
{
	switch (GetCurrentState()) 
	{
	case cUnitIdle:
		break;
	case cUnitMove:
		break;
	case cUnitAttack:
		break;
	case cUnitSoftChase:
		break;
	case cUnitReturn:
		break;
	case cUnitGuard:
		break;
	case cUnitFollow:
		break;
	case cUnitBuild:
		break;
	case cUnitEndBuild:
		break;
	}
}

void BehaviorUnit::OnDestroy()
{
	allUnits.erase(std::find(allUnits.begin(), allUnits.end(), GetParent()));
	GridManager::GetInstance().SetNode(gridPos, true);
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

void BehaviorUnit::UpdatePath()
{
	SetPath(Pathfinder::FindPath(GridManager::GetInstance().GetNode(gridPos), GridManager::GetInstance().GetNode(targetPos)));
}

void BehaviorUnit::Load(rapidjson::Value& obj)
{

}

void BehaviorUnit::UseNone()
{

}

void BehaviorUnit::UseArmor()
{

}

void BehaviorUnit::UseStrobebang()
{

}

void BehaviorUnit::UseEMP()
{

}

// Calculates velocity based off of movement speed, target pos, and current pos.
void BehaviorUnit::CalculateVelocity()
{
	// Calculate a direction vector from current position to taget position.
	Vector2D dir = currMoveTarget->gridPos() - gridPos;
	dir = dir.Normalized();

	dir.y *= -1;

	// Set velocity.
	GetParent()->GetComponent<Physics>()->SetVelocity(dir * (defaultStats.speed * traits.agility));
}

// Checks for enemies within a certain radius of the unit.
// Returns:
// A standard vector containing all of the found units.
std::vector<GameObject*> BehaviorUnit::FindEnemiesInRange() const
{
	std::vector<GameObject*> foundUnits;

	for (GameObject* unit : allUnits)
	{
		if (unit->GetComponent<BehaviorUnit>()->GetArmy() == army)
			continue;

		if (GridManager::GetInstance().IsWithinRange(gridPos, unit->GetComponent<BehaviorUnit>()->GetGridPos(), Weapons[traits.weapon].range))
			foundUnits.push_back(unit);
	}

	return foundUnits;
}

// Checks if the unit can target an enemy (using AttackGroups).
// Params:
//	enemy - the enemy we're checking.
// Returns:
// True if the unit can be targeted, false if not.
bool BehaviorUnit::CanTarget(GameObject* enemy) const
{
	AttackGroup enemyGroup = enemy->GetComponent<BehaviorUnit>()->traits.group;

	switch (Weapons[traits.weapon].group)
	{
	case cGroupMelee:
	case cGroupRanged:
		switch (traits.group)
		{
		case cGroupInfantry:

			break;
		case cGroupAircraft:
			break;
		case cGroupArtillary:
			break;
		}
		if (enemyGroup == cGroupInfantry || enemyGroup == cGroupArtillary)
			return true;
		return false;
	case cGroupLongRanged:
		return true;
	}

	return false;
}

// Checks if the unit can attack an enemy.
// Params:
//	enemy - the enemy we're checking.
// Returns:
// True if the unit can be attacked, false if not.
bool BehaviorUnit::CheckAttack()
{
	// Do we have a target?
	if (target == nullptr)
	{
		// If we are moving to a location, ignore all nearby enemies.
		if (currMoveTarget == nullptr)
			return false;

		// Find all enemies within attack range.
		std::vector<GameObject*> objsInRange = FindEnemiesInRange();

		// Check if we can target each enemy.
		for (GameObject* enemy : objsInRange)
		{
			if (CanTarget(enemy))
			{
				// If we can target this enemy, set state to attack.
				target = enemy;
				SetNextState(cUnitAttack);
				return false;
			}
		}
	}
	else
	{
		// Can we attack our target?
		// if (range < stats.range && target.team != team)
		//	return true;
		// else
		return false;
	}

	return false;
}

unsigned BehaviorUnit::Traits::GetCost()
{
	return 9;
}

void BehaviorUnit::Attack()
{

}

bool BehaviorUnit::IsStuck() const
{
	return false;
}

BehaviorArmy* BehaviorUnit::GetArmy() const
{
	return army;
}

std::vector<GridManager::Node*> BehaviorUnit::GetPath() const
{
	return path;
}

Vector2D BehaviorUnit::GetGridPos() const
{
	return GetNode()->gridPos();
}

void BehaviorUnit::SetGridPos(Vector2D newPos)
{
	gridPos = newPos;
}

Node* BehaviorUnit::GetNode() const
{
	return GridManager::GetInstance().GetNode(gridPos);
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

