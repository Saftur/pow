//------------------------------------------------------------------------------
//
// File Name:	BehaviorUnit.cpp
// Author(s):	Mark Culp
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
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
#include "SoundManager.h"
#include "ColliderCircle.h"

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
500,		// Max HP.
0,			// Defense
5,			// Detection range.
8,			// Disengagement range.
3,			// Inventory size.
10.0f		// Movement speed.
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
	stats.maxHP = (int)(defaultStats.maxHP * (0.05f * traits.strength + 0.15f * traits.defense));
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

	isMoving = true;
}

void BehaviorUnit::BuildArrays()
{
	// Prevent duplicate initialization.
	if (Weapons[0].name == "Drillsaw")
		return;

	///////////////////////////////////////////////////////////////
	// TEST CODE
	SoundManager::GetInstance().AddBank("Master Bank.strings.bank");
	SoundManager::GetInstance().AddBank("Master Bank.bank");
	SoundManager::GetInstance().PlayEvent("Level Music");
	///////////////////////////////////////////////////////////////

	// Build weapon array.
	Weapons[cWeaponDrillsaw] = { "Drillsaw", cGroupMelee, 0.05f, 1, 10, BehaviorProjectile::ProjectileTypes::pTypeInvisible };
	Weapons[cWeaponHandcannon] = { "Handcannon", cGroupRanged, 0.2f, 2, 20, BehaviorProjectile::ProjectileTypes::pTypeLaser };
	Weapons[cWeaponBeamRifle] = { "Beam Rifle", cGroupLongRanged, 0.5f, 3, 30, BehaviorProjectile::ProjectileTypes::pTypeMissile };

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
		GetParent()->GetComponent<ColliderCircle>()->SetCollisionHandler(&CollisionHandler);
		GridManager::GetInstance().GetNode(gridPos)->open = false;
		allUnits.push_back(GetParent());
		SetNextState(cUnitIdle);
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
	switch (GetCurrentState())
	{
	case cUnitIdle:
		if (FindTarget(&target))
			SetNextState(cUnitAttack);
		break;
	case cUnitMove:
		// Set target to null. This ensures that a direct move command from the player always overrides attack.
		target = nullptr;
		if (!isMoving)
			SetNextState(cUnitIdle);
		break;
	case cUnitAttack:
		// Can we attack our target?
		if (FindTarget(&target))
		{
			currMoveTarget = nullptr;
			GetParent()->GetComponent<Physics>()->SetVelocity(Vector2D(0, 0));
			isMoving = false;

			// Can we shoot?
			if (attackTimer <= 0.0f)
			{
				GameObject* projectile = new GameObject(*BehaviorProjectile::Projectiles[Weapons[traits.weapon].projectileArchetype]);
				projectile->GetComponent<Transform>()->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(gridPos));
				projectile->GetComponent<BehaviorProjectile>()->Fire(army, GridManager::GetInstance().ConvertToWorldPoint(target->GetComponent<BehaviorUnit>()->GetGridPos()) - GridManager::GetInstance().ConvertToWorldPoint(gridPos), Weapons[traits.weapon].damage * traits.strength, stats.attackRange);
				GetParent()->GetGameObjectManager()->Add(*projectile);
				// TODO: Make sure this doesn't do shitloads of damage.
				attackTimer = Weapons[traits.weapon].cooldown;
			}
		}
		else
		{
			if (isMoving)
				break;
			if (target)
			{
				// Give chase.
				SetPath(Pathfinder::FindPath(GridManager::GetInstance().GetNode(gridPos), GridManager::GetInstance().GetNode(target->GetComponent<BehaviorUnit>()->GetGridPos())));
			}
			else
			{
				SetNextState(prevState);
			}
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

	if (isMoving)
	{
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
				currMoveTarget = nullptr;
				GetParent()->GetComponent<Physics>()->SetVelocity(Vector2D(0, 0));
				isMoving = false;
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
				GetParent()->GetComponent<Physics>()->SetVelocity(Vector2D(0, 0));
				isMoving = false;

				GameObject* occupant = GridManager::GetInstance().GetOccupant(currMoveTarget);

				currMoveTarget = nullptr;

				if (occupant && occupant->GetComponent<BehaviorUnit>()->GetArmy() != army)
					SetNextState(cUnitAttack);
				else if (!path.empty())
					UpdatePath();

				return;
			}

			currMoveTarget->open = false;

			path.pop_back();
		}


		// Update velocity.
		CalculateVelocity();
	}

	if (attackTimer != 0.0f)
	{
		attackTimer -= dt;
	}

	prevHP = stats.currHP;
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

// Picks a unit to be our target. Returns true if enemy is found, false if not.
bool BehaviorUnit::FindTarget(GameObject** enemy, Vector2D pos) const
{
	if (*enemy)
	{
		// If we already have an enemy, verify that we can still attack them.
		if (!(*enemy)->IsDestroyed())
		{
			if (GridManager::GetInstance().IsWithinRange(gridPos, (*enemy)->GetComponent<BehaviorUnit>()->gridPos, stats.attackRange))
			{
				return true;
			}
		}
		else
		{
			*enemy = nullptr;
		}
	}
	else
	{
		if (pos == Vector2D(-1, -1))
			pos = gridPos;

		Vector2D upperLeft = Vector2D(pos - Vector2D({ (float)stats.attackRange, (float)stats.attackRange }));

		for (int x = (int)upperLeft.x; x < (int)upperLeft.x + 2 * stats.attackRange; x++)
		{
			for (int y = (int)upperLeft.y; y < (int)upperLeft.y + 2 * stats.attackRange; y++)
			{
				GameObject* unit = GridManager::GetInstance().GetOccupant({ (float)x, (float)y });

				if (unit && GridManager::GetInstance().IsWithinRange(pos, { (float)x, (float)y }, stats.attackRange) && unit->GetComponent<BehaviorUnit>()->GetArmy() != army)
				{
					if (CanTarget(unit))
					{
						*enemy = unit;
						return true;
					}
				}
			}
		}
	}

	return false;
}

int BehaviorUnit::GetHP() const
{
	return stats.currHP;
}

void BehaviorUnit::ModifyHP(int amt)
{
	stats.currHP += amt;

	if (stats.currHP > stats.maxHP)
		stats.currHP = stats.maxHP;
	else if (stats.currHP <= 0)
		GetParent()->Destroy();
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

unsigned BehaviorUnit::Traits::GetCost()
{
	return 7;
}

void BehaviorUnit::Attack()
{

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

