//------------------------------------------------------------------------------
//
// File Name:	BuildingTurret.h
// Author(s):	Brand Knutson
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Building.h"


//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

class BuildingTurret : public Building
{
public:
	BuildingTurret(BehaviorArmy::Side side, Vector2D pos);
	Component* Clone() const;

	void BuildingUpdate(float dt); //Update the building.
	
	void OpenMenu(); //Open the upgrade window for the turret.

private:
	void AttackTarget(); //Attack the target unit.
	void FaceTarget(); //Face the target.
	void FindTarget(); //Find a new target.
	bool TargetWithinRange(); //Check if the target is within range.

	GameObject* target = nullptr; //The unit that the turret is targeting.
	Vector2D targetDirection; //The direction vector to the target.
	int range = 5; //The range that the turret can shoot.

	float damage = 100; //How much damage this turret deals.
	float attackSpeed = 3; //How many times per second this turret attacks.
	float attackTimer = 1 / attackSpeed; //Timer used to time attacks.
};

//------------------------------------------------------------------------------
