//------------------------------------------------------------------------------
//
// File Name:	BehaviorUnit.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Behavior.h"
#include "BehaviorProjectile.h"
#include <vector>
#include "Pathfinder.h"

typedef class BehaviorArmy BehaviorArmy;

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

enum WeaponType { cWeaponDrillsaw, cWeaponHandcannon, cWeaponBeamRifle, cNumWeapons };
enum EquipmentType { cEquipNone, cEquipArmor, cEquipStrobebang, cEquipEMP, cNumEquips };
enum Ability { cAbilityNone, cAbilityTechnician, cAbilityEngineer, cAbilityScientist, cNumAbilities };
enum AttackGroup { cGroupAircraft, cGroupInfantry, cGroupArtillary };
enum WeaponGroup { cGroupMelee, cGroupRanged, cGroupLongRanged };

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorUnit.h
class BehaviorUnit : public Behavior
{
public:
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
		BehaviorProjectile::ProjectileTypes projectileArchetype;
		// TODO: Pointers for sounds & maybe art?
	};

	struct Equipment
	{
		std::string name;
		int count;
		void(*use)();
		float deployTime;
		float cooldown;
		// TODO: Pointers for sounds & maybe art?
	};

	/////////////////////
	// Stats
	/////////////////////

	struct Traits
	{
		int strength, agility, defense;

		int ability;
		AttackGroup group;

		int weapon, item1, item2;

		unsigned GetCost();
		std::string name;
	} traits;
	
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Unit) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorUnit();

	BehaviorArmy* GetArmy() const;

	enum states { cUnitIdle, cUnitError, cUnitMove, cUnitAttack, cUnitSoftChase, cUnitReturn, cUnitGuard, cUnitFollow, cUnitBuild, cUnitEndBuild };

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	//   parent = A reference to the parent object (the object that owns this component).
	// Returns:
	//   A pointer to an advanced behavior.
	Component* Clone() const;

	void Init(Traits& data, BehaviorArmy* army);

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

	void OnDestroy();

	// The collision handling function for Units.
	// Params:
	//	 stub = The stub object.
	//	 other = The object the asteroid is colliding with.
	static void CollisionHandler(GameObject& unit, GameObject& other);

	void Load(rapidjson::Value& obj);

	void SetPath(vector<Node*> path);
	vector<Node*> GetPath() const;

	Vector2D GetGridPos() const;
	void SetGridPos(Vector2D newPos);
	Node* GetNode() const;

	int GetHP() const;
	void ModifyHP(int amt);

	bool IsMoving() const;

	void BuildBuilding(GameObject *archetype, Node *pos); //Tell the unit to build a new building.
	GameObject *buildingArchetype; //The archetype of the building we are currently trying to build. nullptr if we are not building.
	Node *buildingPos; //The position of the building we are trying to build. nullptr if we are not building.
	bool isBuilding = false; //If we made it to the spot and are actively building the building.

	static vector<GameObject*> allUnits;

	static Weapon Weapons[cNumWeapons];
	static string AbilityNames[cNumAbilities];
	static Equipment Equips[cNumEquips];

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	bool FindTarget(GameObject** enemy, Vector2D pos = { -1, -1 }) const;

	// Checks if the unit can target an enemy.
	// Params:
	//	enemy - the enemy we're checking.
	// Returns:
	// True if the unit can be targeted, false if not.
	bool CanTarget(GameObject* enemy) const;

	void UpdatePath();

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

	struct BaseStats
	{
		int maxHP;
		int defense;
		int detectRange;
		int disengageRange;
		int inventorySize;
		float speed;
		int baseCost;
	};

	static BaseStats defaultStats;

	struct
	{
		int maxHP;
		int currHP;
		float defense;
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
	Vector2D targetPos, lastMoveTarget, guardingPos;
	vector<Node*> path;
	Node* currMoveTarget;
	states prevState;
	float stuckTimer, attackTimer;
	Vector2D gridPos;
	bool isMoving = false;

	bool changePath = false;
	vector<Node*> changedPath;

	BehaviorArmy* army;
};

//------------------------------------------------------------------------------
