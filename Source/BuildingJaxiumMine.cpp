//------------------------------------------------------------------------------
//
// File Name:	BuildingJaxiumMine.cpp
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "BuildingJaxiumMine.h"
#include "GameObjectManager.h"
#include "Space.h"
#include "LevelManager.h"
#include "GridManager.h"
#include <vector>
#include "Crystal.h"

using std::vector;

BuildingJaxiumMine::BuildingJaxiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, JaxiumMine, Basic, 3.5f, 1000.0f, pos, 50, 0)
{
}

Component * BuildingJaxiumMine::Clone() const
{
	return new BuildingJaxiumMine(*this);
}

bool BuildingJaxiumMine::AddWorker()
{
	if (workers < 3) {
		workers++;
		return true;
	}
	return false;
}

bool BuildingJaxiumMine::RemoveWorker()
{
	if (workers > 0) {
		workers--;
		return true;
	}
	return false;
}

void BuildingJaxiumMine::BuildingUpdate(float dt){
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
			if (Space::GetLayer(0)->GetGameObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
				if (obj->GetComponent<Crystal>()) {
					if (obj->GetComponent<Transform>()->GetTranslation() == GridManager::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID])) {
						return true;
					}
				}
				return false;
			}).size() > 0) return;

			remainingSpawnTime = Variance(crystalSpawnTime, crystalSpawnTimeVariance) / workers; //Reset the spawn timer.

			//Create a jaxium crystal on the node.
			GameObject* jaxium = new GameObject(*Space::GetLayer(0)->GetGameObjectManager()->GetArchetype("Jaxium Archetype"));
			jaxium->AddComponent(new Crystal(Crystal::CrystalType::Jaxium, Variance(fundsPerCrystal, crystalWorthVariance)));
			jaxium->GetComponent<Transform>()->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID]));

			Space::GetLayer(0)->GetGameObjectManager()->Add(*jaxium);
		}
	}
}
