//------------------------------------------------------------------------------
//
// File Name:	BehaviorUnit.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Behavior.h"
#include <vector>
#include "Pathfinding.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// An example of the enums to be defined in BehaviorUnit.cpp.
#if 0
enum UnitState
{
};
#endif

enum SpecialtyType { Basic, Advanced, Special };

class Building
{
	SpecialtyType specialtyType;
	float buildTime;
};

enum WeaponType { cWeaponDrillsaw, cWeaponHandcannon, cWeaponBeamRifle, cNumWeapons };
enum EquipmentType { cEquipNone, cEquipArmor, cEquipStrobebang, cEquipEMP, cNumEquips };
enum Ability { cAbilityNone, cAbilityTechnician, cAbilityEngineer, cAbilityScientist };
enum AttackGroup { cGroupAircraft, cGroupInfantry, cGroupArtillary };
enum WeaponGroup { cGroupMelee, cGroupRanged, cGroupLongRanged };

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorUnit.h
class BehaviorUnit : public Behavior
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Unit) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorUnit(int strength, int agility, int defense, Ability ability, WeaponType weapon, EquipmentType item1, EquipmentType item2);

private:
	enum states { cUnitIdle, cUnitMove, cUnitAttack, cUnitSoftChase, cUnitReturn, cUnitGuard, cUnitFollow, cUnitBuild, cUnitEndBuild };

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	//   parent = A reference to the parent object (the object that owns this component).
	// Returns:
	//   A pointer to an advanced behavior.
	Component* Clone() const;

	// Initialize the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	void OnEnter();

	// Update the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	//	 dt = Change in time (in seconds) since the last game loop.
	void OnUpdate(float dt);

	void OnExit();

	// The collision handling function for Units.
	// Params:
	//	 stub = The stub object.
	//	 other = The object the asteroid is colliding with.
	static void CollisionHandler(GameObject& stub, GameObject& other);

	void Load(rapidjson::Value& obj);

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------
	// Initializes this unit's stats based off of its traits.
	void CalculateStats();

	// Checks for enemies within a certain radius of the unit.
	// Returns:
	// A standard vector containing all of the found units.
	std::vector<GameObject*> FindEnemiesInRange();

	// Checks if the unit can target an enemy.
	// Params:
	//	enemy - the enemy we're checking.
	// Returns:
	// True if the unit can be targeted, false if not.
	bool CanTarget(GameObject* enemy);

	// Checks if the unit can attack an enemy.
	// Params:
	//	enemy - the enemy we're checking.
	// Returns:
	// True if the unit can be attacked, false if not.
	bool CheckAttack();

	// Calculates velocity based off of movement speed, target pos, and current pos.
	void CalculateVelocity();

	// Builds the weapon and equipment arrays.
	void BuildArrays();

	// Equipment use functions.
	static void UseNone();
	static void UseArmor();
	static void UseStrobebang();
	static void UseEMP();

	void Attack();

	//------------------------------------------------------------------------------
	// Stats
	//------------------------------------------------------------------------------
	
	/////////////////////
	// Items
	/////////////////////
	struct Weapon
	{
		std::string name;
		WeaponGroup group;
		float cooldown;
		int range;
		int damage;
		GameObject* projectileArchetype;
		// TODO: Pointers for sounds & maybe art?
	};
	
	struct Equipment
	{
		std::string name;
		int count;
		void (*use)();
		float deployTime;
		float cooldown;
		// TODO: Pointers for sounds & maybe art?
	};

	/////////////////////
	// Stats
	/////////////////////
	struct 
	{
		int strength;
		int agility;
		int defense;
		Ability ability;
		AttackGroup group;
		Weapon weapon;
		Equipment item1;
		Equipment item2;
	} traits;

	static struct BaseStats
	{
		int maxHP;
		int defense;
		int detectRange;
		int disengageRange;
		int inventorySize;
		float speed;
	};

	static BaseStats defaultStats;

	struct
	{
		int maxHP;
		int currHP;
		int defense;
		int detectRange; // increase on high ground
		int attackRange; // increase on high ground
		int inventorySize;
	} stats;

	/////////////////////
	// Misc/placeholder
	/////////////////////

	//------------------------------------------------------------------------------
	// Data
	//------------------------------------------------------------------------------
	GameObject* target;
	int prevHP;
	Vector2D targetPos, lastFrameTarget, guardingPos;
	std::vector<Grid::Node*> path;
	Grid::Node* moveTarget;
	states prevState;

	static Weapon Weapons[cNumWeapons];
	static Equipment Equips[cNumEquips];
};

//------------------------------------------------------------------------------
