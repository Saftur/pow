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
#include "Teleporter.h"
#include "BehaviorUnit.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Pathfinding.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

std::vector<GameObject*> units;

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

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Unit) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorUnit::BehaviorUnit(int strength, int agility, int defense, Ability ability, WeaponType weapon, EquipmentType item1, EquipmentType item2) :
	Behavior("BehaviorUnit")
{
	SetCurrentState(cBehaviorInvalid);
	SetNextState(cUnitIdle);

	// Build the static arrays if they haven't already been built.
	BuildArrays();

	// Initialize this unit's traits.
	traits.ability = ability;
	traits.strength = strength;
	traits.agility = agility;
	traits.defense = defense;
	traits.weapon = Weapons[weapon];
	traits.item1 = Equips[item1];
	traits.item2 = Equips[item2];
}

void BehaviorUnit::CalculateStats()
{
	stats.attackRange = traits.weapon.range;
	stats.maxHP = defaultStats.maxHP + 0.05f * traits.strength + 0.15f * traits.defense;
	stats.currHP = stats.maxHP;
	stats.inventorySize = defaultStats.inventorySize + 0.5f * traits.strength + 0.5f * traits.agility;
	stats.detectRange = defaultStats.detectRange;
	stats.defense = defaultStats.defense + 0.05f * traits.defense;

	// If there's armor, apply an HP boost. If there's an empty slot, give an inventory size boost.
	if (traits.item1.name == Equips[cEquipArmor].name)
	{
		stats.currHP += 50;
	}
	else if (traits.item1.name == Equips[cEquipNone].name)
	{
		stats.inventorySize += 2;
	}

	if (traits.item2.name == Equips[cEquipArmor].name)
	{
		stats.currHP += 50;
	}
	else if (traits.item2.name == Equips[cEquipNone].name)
	{
		stats.inventorySize += 2;
	}
}

void BehaviorUnit::BuildArrays()
{
	// Prevent duplicate initialization.
	if (Weapons[0].name == "Drillsaw")
		return;

	// Build weapon array.
	Weapons[cWeaponDrillsaw] = { "Drillsaw", cGroupMelee, 0.05f, 1, 10, GetParent()->GetObjectManager()->GetArchetype("ProjectileInvisible") };
	Weapons[cWeaponHandcannon] = { "Handcannon", cGroupRanged, 0.2f, 3, 20, GetParent()->GetObjectManager()->GetArchetype("ProjectileLaser") };
	Weapons[cWeaponBeamRifle] = { "Beam Rifle", cGroupLongRanged, 0.5f, 5, 30, GetParent()->GetObjectManager()->GetArchetype("ProjectileLaser") };

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
	switch (GetCurrentState())
	{
	case cUnitIdle:
		CalculateStats();
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
		// If our target has changed, we need to calculate a new path.
		if (lastFrameTarget != targetPos)
		{
			path.clear();
		}

		// Do we need to do pathfinding?
		if (path.empty())
		{
			Grid::Node* start = Grid::GetInstance().GetNode(((Transform*)GetParent()->GetComponent("Transform"))->GetWorldTranslation());
			Grid::Node* end = Grid::GetInstance().GetNode(targetPos);

			path = Pathfinding::FindPath(start, end);

			// Set the first node.
			moveTarget = path.back();
			path.pop_back();
		}

		// Have we reached a node?
		if (Grid::GetInstance().GetNode(((Transform*)GetParent()->GetComponent("Transform"))->GetWorldTranslation()) == moveTarget)
		{
			// Set the next node.
			delete (((moveTarget)));
			moveTarget = path.back();
			path.pop_back();
		}

		// If pathfinding failed, set state to Idle.
		if (path.empty())
		{
			SetNextState(cUnitIdle);
			target = nullptr;
			return;
		}

		// Update velocity
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

// The collision handling function for Units.
// Params:
//	 stub = The stub object.
//	 other = The object the asteroid is colliding with.
void BehaviorUnit::CollisionHandler(GameObject& stub, GameObject& other)
{
	UNREFERENCED_PARAMETER(stub);
	UNREFERENCED_PARAMETER(other);
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

void BehaviorUnit::Attack()
{
}

// Calculates velocity based off of movement speed, target pos, and current pos.
void BehaviorUnit::CalculateVelocity()
{
	// Calculate a direction vector from current position to taget position.
	Vector2D dir = moveTarget->worldPos - ((Transform*)GetParent()->GetComponent("Transform"))->GetTranslation();
	dir.Normalized();

	// Set velocity.
	((Physics*)GetParent()->GetComponent("Physics"))->SetVelocity(dir * (defaultStats.speed * traits.agility));
}

// Checks for enemies within a certain radius of the unit.
// Returns:
// A standard vector containing all of the found units.
std::vector<GameObject*> BehaviorUnit::FindEnemiesInRange()
{
	std::vector<GameObject*> foundUnits;

	for (GameObject* unit : units)
	{
		// if unit.team != team
		// if dist(unit.gridPos <= traits.weapon.range) foundUnits.push_back(unit);
		foundUnits.push_back(unit);
	}

	return foundUnits;
}

// Checks if the unit can target an enemy.
// Params:
//	enemy - the enemy we're checking.
// Returns:
// True if the unit can be targeted, false if not.
bool BehaviorUnit::CanTarget(GameObject* enemy)
{
	AttackGroup enemyGroup = ((BehaviorUnit*)enemy->GetComponent("Behavior"))->traits.group;

	switch (traits.weapon.group)
	{
	case cGroupMelee:
	case cGroupRanged:
		if (enemyGroup == cGroupInfantry || enemyGroup == cGroupArtillary)
			return true;
		return false;
	case cGroupLongRanged:
		return true;
	}
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
		if (moveTarget == nullptr)
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
		//	return false;
	}
}

void BehaviorUnit::Attack()
{

}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

