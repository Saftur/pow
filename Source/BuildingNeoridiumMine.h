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
	static float GetNeoridium(BehaviorArmy::Side side); //Get current of neoridium owned by given army.

	bool AddWorker(); //Add a worker to the mine if there arent already too many workers.
	bool RemoveWorker(); //Remove a worker from the mine if there is at least one worker.

	static std::map<BehaviorArmy::Side, float> neoridium; //How much neoridium each army has.

private:
	float fundsPerCrystal = 10; //How much each Neoridium crystal is worth.

	float crystalSpawnTime = 10; //How long it takes for a crystal to spawn. (HINT: This is divided by the number of workers).
	float remainingSpawnTime = crystalSpawnTime; //How long until the next crystal will spawn.

	float workers = 3; //How many units are working this mine.
	float maxWorkers = 3; //The maximum number of workers on this mine.

	float crystalWorthVariance = 0.5f; //The percentage that the value of each crystal can vary by.
	float crystalSpawnTimeVariance = 0.3f; //The percentage that the spawn time of each crystal can vary by.
};

//------------------------------------------------------------------------------
