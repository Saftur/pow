//------------------------------------------------------------------------------
//
// File Name:	BuildingTeleporter.h
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
#include <map>

using std::map;

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

class BuildingTeleporter : public Building
{
public:
	BuildingTeleporter(BehaviorArmy::Side side, Vector2D pos);
	~BuildingTeleporter();

	Component* Clone() const;

	void BuildingUpdate(float dt); //Update the building.

	bool SetExit(); //Set the exit on this teleporter to another one found, if it exists, and the exit on that one to this one.

	static map<BehaviorArmy::Side, unsigned> teleporters; //The number of teleporters that each army has.

private:
	unsigned teleportationsAvailable = 5; //How many times this teleporter can still teleport units.

	GameObject* exit = nullptr; //The teleporter that this teleporter routes to.
	GameObject* lastTeleportedObject = nullptr; //Pointer to the last teleported object.
};

//------------------------------------------------------------------------------
