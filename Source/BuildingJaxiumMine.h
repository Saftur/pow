//------------------------------------------------------------------------------
//
// File Name:	BuildingJaxiumMine.h
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

class BuildingJaxiumMine : public Building
{
public:
	BuildingJaxiumMine(BehaviorArmy::Side side, Vector2D pos);
	Component* Clone() const;

	bool AddWorker(); //Add a worker to the mine if there arent already too many workers.
	bool RemoveWorker(); //Remove a worker from the mine if there is at least one worker.

	void BuildingUpdate(float dt); //Update the building.

private:
	float fundsPerCrystal = 20; //How much each Jaxium crystal is worth.
	
	float crystalSpawnTime = 10; //How long it takes for a crystal to spawn. (HINT: This is divided by the number of workers).
	float remainingSpawnTime = crystalSpawnTime; //How long until the next crystal will spawn.

	float workers = 3; //How many units are working this mine.
	float maxWorkers = 3; //The maximum number of workers on this mine.

	float crystalWorthVariance = 0.5f; //The percentage that the value of each crystal can vary by.
	float crystalSpawnTimeVariance = 0.3f; //The percentage that the spawn time of each crystal can vary by.
};

//------------------------------------------------------------------------------
