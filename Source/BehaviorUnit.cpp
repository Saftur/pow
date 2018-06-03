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
#include "SoundManager.h"
#include "ColliderCircle.h"
#include "Health.h"
#include "Rendertext.h"
#include "BehaviorArmy.h"
#include "Building.h"
#include "Space.h"

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

BehaviorUnit::Weapon BehaviorUnit::Weapons[cNumWeapons] = {
	{ "Drillsaw", cGroupMelee, 0.05f, 1, 10, BehaviorProjectile::ProjectileTypes::pTypeInvisible },
	{ "Handcannon", cGroupRanged, 0.2f, 2, 20, BehaviorProjectile::ProjectileTypes::pTypeLaser },
	{ "Beam Rifle", cGroupLongRanged, 0.5f, 3, 30, BehaviorProjectile::ProjectileTypes::pTypeMissile }
};
string BehaviorUnit::AbilityNames[cNumAbilities] = {"None", "Technician", "Engineer", "Scientist"};
BehaviorUnit::Equipment BehaviorUnit::Equips[cNumEquips] = {
	{ "null", 0, UseNone, 0.0f, 0.0f },
	{ "Diamondium Armor", -1, UseArmor, 0.0f, 0.0f },
	{ "Strobebang", 5, UseStrobebang, 1.0f, 30.0f },
	{ "EMP", 5, UseEMP, 1.0f, 30.0f }
};

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

	Text* name = GetParent()->GetComponent<Text>();
	name->SetText(theTraits.name.c_str());
	
	if (theArmy->GetSide() == BehaviorArmy::Side::sRight)
		name->SetColor({ 0.f, 0.f, 1.f, 1.f });

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

	GetParent()->GetComponent<Health>()->Initialize(stats.maxHP);
}

void BehaviorUnit::SetPath(std::vector<Node*> newPath)
{
	//If we are actually building something, we do not want to be able to move.
	if (isBuilding) return;

	//If we are moving to build something, stop trying to build it.
	if (buildingArchetype) {
		buildingArchetype = nullptr;
		GridManager::GetInstance().SetNode(buildingPos, true);
		buildingPos = nullptr;
	}

	changePath = false;
	if (isMoving) {
		changePath = true;
		changedPath = newPath;
	}
	else {
		path = newPath;
		if (currMoveTarget) currMoveTarget->open = true;
		currMoveTarget = GridManager::GetInstance().GetNode(gridPos);

		SetNextState(cUnitMove);
		isMoving = true;
		if (path.size() > 0)
			targetPos = newPath[0]->gridPos();
		else
			targetPos = gridPos;
	}
}

void BehaviorUnit::BuildArrays()
{
	// Prevent duplicate initialization.
	static bool done = false;
	if (done)
		return;
	done = true;

	///////////////////////////////////////////////////////////////
	// TEST CODE
	SoundManager::GetInstance().AddBank("Master Bank.strings.bank");
	SoundManager::GetInstance().AddBank("Master Bank.bank");
	SoundManager::GetInstance().PlayEvent("Level Music");
	///////////////////////////////////////////////////////////////
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

				BehaviorUnit* bUnit = target->GetComponent<BehaviorUnit>();
				Building* building = target->GetChildComponent<Building>();

				if(bUnit) projectile->GetComponent<BehaviorProjectile>()->Fire(army, GridManager::GetInstance().ConvertToWorldPoint(bUnit->GetGridPos()) - GridManager::GetInstance().ConvertToWorldPoint(gridPos), Weapons[traits.weapon].damage * traits.strength, stats.attackRange);
				else if(building) projectile->GetComponent<BehaviorProjectile>()->Fire(army, GridManager::GetInstance().ConvertToWorldPoint(building->mapPos) - GridManager::GetInstance().ConvertToWorldPoint(gridPos), Weapons[traits.weapon].damage * traits.strength, stats.attackRange);
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
			if (changePath) {
				path = changedPath;
				changePath = false;
			}

			if (currMoveTarget)
				currMoveTarget->open = true;

			// Do we have more movements to make?
			if (path.empty())
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

				BehaviorArmy *oArmy = nullptr;
				if (occupant) {
					BehaviorUnit *bUnit = occupant->GetComponent<BehaviorUnit>();
					Building *bBuilding = occupant->GetChildComponent<Building>();

					if (bUnit) oArmy = bUnit->GetArmy();
					else if (bBuilding) oArmy = bBuilding->army;
				}
				else {
					//Check if we are trying to build something there.
					if (buildingArchetype) {
						GridManager::GetInstance().SetNode(buildingPos, true);
						if (army->LegalSpawn(buildingPos->gridPos())) {
							isBuilding = true;

							GameObject *newBuilding = new GameObject(*buildingArchetype);
							Building *building = newBuilding->GetChildComponent<Building>();
							building->SetPos(buildingPos->gridPos());
							building->SetArmy(army);
							building->Buy();
							building->builder = GetParent();
							Space::GetLayer(0)->GetGameObjectManager()->Add(*newBuilding);

							buildingArchetype = nullptr;
							buildingPos = nullptr;

							return;
						}
					}
				}

				if (occupant && oArmy != army)
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
	if (std::find(allUnits.begin(), allUnits.end(), GetParent()) != allUnits.end())
		allUnits.erase(std::find(allUnits.begin(), allUnits.end(), GetParent()));
	GridManager::GetInstance().SetNode(gridPos, true);
	if(currMoveTarget) GridManager::GetInstance().SetNode(currMoveTarget, true);
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
			BehaviorUnit* bUnit = (*enemy)->GetComponent<BehaviorUnit>();
			Building* building = (*enemy)->GetChildComponent<Building>();

			if (bUnit && GridManager::GetInstance().IsWithinRange(gridPos, bUnit->gridPos, stats.attackRange))
			{
				return true;
			}
			else if (building && GridManager::GetInstance().IsWithinRange(gridPos, building->mapPos, stats.attackRange)) return true;
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
				GameObject* unit = GridManager::GetInstance().GetOccupant(Vector2D( (float)x, (float)y ));

				if (unit) {
					BehaviorUnit* bUnit = unit->GetComponent<BehaviorUnit>();
					Building* building = unit->GetChildComponent<Building>();

					if (GridManager::GetInstance().IsWithinRange(pos, { (float)x, (float)y }, stats.attackRange) && ((bUnit && bUnit->GetArmy() != army) || building && building->army != army))
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
	}

	return false;
}

int BehaviorUnit::GetHP() const
{
	return stats.currHP;
}

void BehaviorUnit::ModifyHP(int amt)
{
	if (stats.currHP <= 0) return;
	stats.currHP += amt;
	GetParent()->GetComponent<Health>()->UpdateHP(amt);
}

bool BehaviorUnit::IsMoving() const
{
	return isMoving;
}

void BehaviorUnit::BuildBuilding(GameObject * archetype, Node * pos)
{
	SetPath(Pathfinder::FindPath(GridManager::GetInstance().GetNode(gridPos), pos));
	GridManager::GetInstance().SetNode(pos, false);
	buildingArchetype = archetype;
	buildingPos = pos;
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
	Building* building = enemy->GetChildComponent<Building>();
	AttackGroup enemyGroup;
	if (building) enemyGroup = cGroupInfantry;
	else enemyGroup = enemy->GetComponent<BehaviorUnit>()->traits.group;

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
	GridManager::GetInstance().SetNode(gridPos, true);
	GridManager::GetInstance().SetNode(newPos, false);
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

