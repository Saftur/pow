#include "stdafx.h"
#include "Building.h"
#include "LevelManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "AEEngine.h"
#include "Transform.h"

#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"

map<BehaviorArmy::Side, bool[Building::BuildingType::BuildingCount]> Building::buildings;

Building::Building(BehaviorArmy::Side side, BuildingType type, SpecialtyType specialtyType, float buildTime, float buildCost, CostType costType, Vector2D pos) 
	: Component("Building"), buildingType(type), specialtyType(specialtyType), buildTime(buildTime), side(side), buildCost(buildCost)
{
	//Find the army that this building should belong to.
	vector<GameObject*> objs = LevelManager::GetLayer(0)->GetObjectManager()->GetObjectsByName("Army");
	for (unsigned i = 0; i < objs.size(); i++) {
		BehaviorArmy* bArmy = (BehaviorArmy*)objs[i]->GetComponent("BehaviorArmy");
		if (bArmy->GetSide() == side) {
			army = bArmy;
			if (!army->LegalSpawn(pos)) throw(0); //Throw an error if the slot is occupied.
			if (costType == Jaxium && !bArmy->TakeFromFunds(buildCost)) throw(0); //Throw an error if there is not enough Jaxium to build the building.
			if (costType == Neoridium && !BuildingNeoridiumMine::TakeNeoridium(side, buildCost)) throw(0); //Throw an error if there is not enough Neoridium to build the building.
			buildTimeRemaining = buildTime; //Set the delay until the building is finnished being built.
			originalScale = { 0, 0 }; //Default to 0 so we can check if we set it yet or not on update when we actually have the transform component attatched.
		}
	}
}

Building::~Building()
{
	if (texture) AEGfxTextureUnload(texture);
	if (mesh) AEGfxMeshFree(mesh);
}

void Building::InitializeBuildings(BehaviorArmy::Side side)
{
	BuildingResearchCenter::InitializeResearchCost();

	for (int i = 0; i < BuildingCount; i++) Lock(side, (BuildingType)i); //Lock all of the buildings.
	Unlock(side, JaxiumMine);
	Unlock(side, ResearchCenter);
	///TODO: Remove further unlocks, their only purpose is for testing.
	Unlock(side, NeoridiumMine);

	BuildingNeoridiumMine::neoridium[side] = 0.0f; //Intitialize the amount of Neoridium each player has to 0.
}

void Building::Update(float dt)
{
	//If the building hasn't finnished being built yet, don't update it.
	if (buildTimeRemaining > 0) {
		buildTimeRemaining -= dt; //Lower the time until the building is finnished being built.

		//If we havent set the original scale, set it now and then set the scale to 0.
		if (originalScale == Vector2D(0, 0)) {
			originalScale = GetParent()->GetComponent<Transform>()->GetScale();
			GetParent()->GetComponent<Transform>()->SetScale({ 0, 0 });
		}
		//Increase the size until it reaches the size that it should be. (Should occur when buildTimeRemaining = 0).
		else {
			Transform* transform = GetParent()->GetComponent<Transform>();
			transform->SetScale(transform->GetScale() + Vector2D((originalScale.x / buildTime) * dt, (originalScale.y / buildTime) * dt));
		}
	}
	else {
		BuildingUpdate(dt); //Update the building.
	}
}

void Building::Lock(BehaviorArmy::Side side, BuildingType type)
{
	buildings[side][type] = false;
}

void Building::Unlock(BehaviorArmy::Side side, BuildingType type)
{
	buildings[side][type] = true;
}

bool Building::IsUnlocked(BehaviorArmy::Side side, BuildingType type)
{
	return buildings[side][type];
}
