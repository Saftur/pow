//------------------------------------------------------------------------------
//
// File Name:	BuildingNeoridiumMine.cpp
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BuildingNeoridiumMine.h"
#include "Space.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "GameObject.h"
#include "Crystal.h"
#include "AEEngine.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "SoundManager.h"

std::map<BehaviorArmy::Side, float> BuildingNeoridiumMine::neoridium;

BuildingNeoridiumMine::BuildingNeoridiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, NeoridiumMine, Advanced, 3.5f, pos, 0, 50)
{
}

Component * BuildingNeoridiumMine::Clone() const
{
	return new BuildingNeoridiumMine(*this);
}

void BuildingNeoridiumMine::BuildingUpdate(float dt){
	//If there is at least one unit working this mine.
	if (workers > 0) {
		//Decrement remaining spawn time for a crystal.
		remainingSpawnTime -= dt;

		//If a crystal is ready, spawn it.
		if (remainingSpawnTime <= 0) {

			//Get a list of all nearby nodes.
			vector<GridManager::Node*> nearbyNodes = GridManager::GetInstance().GetNeighbors(GridManager::GetInstance().GetNode((int)mapPos.x, (int)mapPos.y));

			//Find a random open node in the list of nearby nodes.
			unsigned nodeID = rand() % nearbyNodes.size();
			if (GridManager::GetInstance().GetOccupant<Building>(nearbyNodes[nodeID])) return; //Make sure there isn't a building there.
			if (Space::GetLayer(0)->GetGameObjectManager()->CountObjectsWithFilter([&](GameObject* obj) {
				if (obj->GetComponent<Crystal>()) {
					if (obj->GetComponent<Transform>()->GetTranslation() == GridManager::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID])) {
						return true;
					}
				}
				return false;
			}) > 0) return;

			remainingSpawnTime = Variance(crystalSpawnTime, crystalSpawnTimeVariance) / workers; //Reset the spawn timer.

			//Create a neoridium crystal on the node.
			GameObject* neoridium = new GameObject(*Space::GetLayer(0)->GetGameObjectManager()->GetArchetype("Neoridium Archetype"));
			neoridium->AddComponent(new Crystal(Crystal::CrystalType::Neoridium, Variance(fundsPerCrystal, crystalWorthVariance)));
			neoridium->GetComponent<Transform>()->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID]));

			Space::GetLayer(0)->GetGameObjectManager()->Add(*neoridium);
		}
	}
	if (!firstRun)
		return;
	firstRun = false;
	SoundManager::GetInstance().Add("Neoridium_Mine.wav");
	SoundManager::GetInstance().PlaySFX("Neoridium_Mine.wav");
}

bool BuildingNeoridiumMine::TakeNeoridium(BehaviorArmy::Side side, float amount)
{
	if (neoridium[side] >= amount) {
		neoridium[side] -= amount;
		return true;
	}
	return false;
}

void BuildingNeoridiumMine::AddNeoridium(BehaviorArmy::Side side, float amount)
{
	neoridium[side] += amount;
}

float BuildingNeoridiumMine::GetNeoridium(BehaviorArmy::Side side) {
	return neoridium[side];
}

bool BuildingNeoridiumMine::AddWorker()
{
	if (workers < 3) {
		workers++;
		return true;
	}
	return false;
}

bool BuildingNeoridiumMine::RemoveWorker()
{
	if (workers > 0) {
		workers--;
		return true;
	}
	return false;
}
