//------------------------------------------------------------------------------
//
// File Name:	BuildingNeoridiumMine.h
// Author(s):	Brand Knutson
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Building.h"
#include "BehaviorArmy.h"


//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

class BuildingNeoridiumMine : public Building
{
public:
	BuildingNeoridiumMine(BehaviorArmy::Side side, Vector2D pos);
	Component* Clone() const;

	void BuildingUpdate(float dt); //Update the building.

	static bool TakeNeoridium(BehaviorArmy::Side side, float amount); //Take neoridium from the given army if it has enough to give it.
	static void AddNeoridium(BehaviorArmy::Side side, float amount); //Give neoridium to the given army.

	static std::map<BehaviorArmy::Side, float> neoridium; //How much neoridium each army has.

private:
	unsigned fundsPerSecond = 10; //How much Neoridium does this mine generate per second.
};

//------------------------------------------------------------------------------
