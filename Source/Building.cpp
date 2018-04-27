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

map<Building::BuildingType, float> Building::buildingCost;

Building::Building(BehaviorArmy::Side side, BuildingType type, SpecialtyType specialtyType, float buildTime, float maxHealth, Vector2D pos) 
	: Component("Building"), buildingType(type), specialtyType(specialtyType), buildTime(buildTime), side(side), maxHealth(maxHealth), health(maxHealth)
{
	//Find the army that this building should belong to.
	vector<GameObject*> objs = LevelManager::GetLayer(0)->GetObjectManager()->GetObjectsByName("Army");
	for (unsigned i = 0; i < objs.size(); i++) {
		BehaviorArmy* bArmy = (BehaviorArmy*)objs[i]->GetComponent("BehaviorArmy");
		if (bArmy->GetSide() == side) {
			army = bArmy;
			if (!army->LegalSpawn(pos)) throw(0); //Throw an error if the slot is occupied.
			if (!army->TakeFromFunds(buildingCost[type])) throw(0); //Throw an error if we can't pay for the building. (This should never happen).
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

	//Inititialize the cost of buildings.
	buildingCost[JaxiumMine] = 100.0f;
	buildingCost[NeoridiumMine] = 200.0f;
	buildingCost[ResearchCenter] = 250.0f;
	buildingCost[Spaceport] = 350.0f;
	buildingCost[VehicleDepot] = 300.0f;
	buildingCost[Turret] = 200.0f;
	buildingCost[Teleporter] = 200.0f;
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

	//Destroy the building and drop some money if it's health drops below 0.
	if (health <= 0) {
		GetParent()->Destroy();

		///TODO: Drop some money for the other team.
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

void Building::SetHealth(float amount)
{
	health = amount < maxHealth ? amount : maxHealth;
}

float Building::GetHealth()
{
	return health;
}

void Building::Damage(float amount)
{
	health -= amount;
}

void Building::Heal(float amount)
{
	health = (health + amount) < maxHealth ? (health + amount) : maxHealth;
}
