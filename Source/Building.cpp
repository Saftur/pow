#include "stdafx.h"
#include "Building.h"
#include "Space.h"
#include "LevelManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "AEEngine.h"
#include "Transform.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "Crystal.h"
#include <time.h>

#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingCommandPost.h"
#include "BuildingTeleporter.h"
#include <algorithm>

map<BehaviorArmy::Side, bool[Building::BuildingType::BuildingCount]> Building::buildings;

map<Building::BuildingType, float> Building::buildingCost;
vector<GameObject*> Building::allBuildings;

Building::Building(BehaviorArmy::Side side, BuildingType type, SpecialtyType specialtyType, float buildTime, float maxHealth, Vector2D pos, float jaxiumDropAmount, 
	float neoridiumDropAmount) : Component("Building"), buildingType(type), specialtyType(specialtyType), buildTime(buildTime), mapPos(pos), side(side), maxHealth(maxHealth),
	health(maxHealth), jaxiumDropAmount(jaxiumDropAmount), neoridiumDropAmount(neoridiumDropAmount)
{
	//Find the army that this building should belong to.
	vector<GameObject*> objs = Space::GetLayer(0)->GetGameObjectManager()->GetObjectsByName("Army");
	for (unsigned i = 0; i < objs.size(); i++) {
		BehaviorArmy* bArmy = (BehaviorArmy*)objs[i]->GetComponent("BehaviorArmy");
		if (bArmy->GetSide() == side) {
			army = bArmy;
			if (specialtyType != sCommandPost && !army->LegalSpawn(pos)) throw(0); //Throw an error if the slot is occupied.
			if (type == Teleporter) if (!BuildingNeoridiumMine::TakeNeoridium(side, buildingCost[type])) throw(0);
			else if (!army->TakeFromFunds(buildingCost[type])) throw(0); //Throw an error if we can't pay for the building. (This should never happen).
			buildTimeRemaining = buildTime; //Set the delay until the building is finnished being built.
			originalScale = { 0, 0 }; //Default to 0 so we can check if we set it yet or not on update when we actually have the transform component attatched.
		}
	}
}

Building::~Building()
{
	if (texture) AEGfxTextureUnload(texture);
	if (mesh) AEGfxMeshFree(mesh);
	allBuildings.erase(std::remove(allBuildings.begin(), allBuildings.end(), GetParent()), allBuildings.end());
}

void Building::InitializeBuildings(BehaviorArmy::Side side)
{
	srand((unsigned) time(NULL));
	BuildingResearchCenter::InitializeResearchCost();

	for (int i = 0; i < BuildingCount; i++) Lock(side, (BuildingType)i); //Lock all of the buildings.
	Unlock(side, JaxiumMine);
	Unlock(side, ResearchCenter);
	///TODO: Remove further unlocks, their only purpose is for testing.
	Unlock(side, NeoridiumMine); //Get's unlocked when you first build a Research Center.
	Unlock(side, Teleporter);

	BuildingNeoridiumMine::neoridium[side] = 0.0f; //Intitialize the amount of Neoridium each player has to 0.

	//Inititialize the cost of buildings.
	buildingCost[JaxiumMine] = 250.0f;
	buildingCost[NeoridiumMine] = 200.0f;
	buildingCost[ResearchCenter] = 250.0f;
	buildingCost[Spaceport] = 350.0f;
	buildingCost[VehicleDepot] = 300.0f;
	buildingCost[Turret] = 200.0f;
	buildingCost[Teleporter] = 0.0f;
	buildingCost[CommandPost] = -0.0f;

	//Initialize the number of teleporters each army has to 0.
	BuildingTeleporter::teleporters[side] = 0;
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
			allBuildings.push_back(GetParent());
		}
		//Increase the size until it reaches the size that it should be. (Should occur when buildTimeRemaining = 0).
		else {
			Transform* transform = GetParent()->GetComponent<Transform>();
			transform->SetScale(transform->GetScale() + Vector2D((originalScale.x / buildTime) * dt, (originalScale.y / buildTime) * dt));
			if (transform->GetScale().x > originalScale.x || transform->GetScale().y > originalScale.y) transform->SetScale(originalScale);
		}
	}
	else {
		BuildingUpdate(dt); //Update the building.
	}

	//Destroy the building and drop some money if it's health drops below 0.
	if (health <= 0) {
		GetParent()->Destroy();

		//Drop some money.
		vector<GridManager::Node*> nearbyNodes = GridManager::GetInstance().GetNeighbors(GridManager::GetInstance().GetNode((int)mapPos.x, (int)mapPos.y)); //Find all neaby nodes.
		unsigned nodeID = rand() % nearbyNodes.size(); //Pick a random node out of the list of nearby nodes.

		if (jaxiumDropAmount > 0) {
			//Create a jaxium crystal on the node.
			GameObject* jaxium = new GameObject(*Space::GetLayer(0)->GetGameObjectManager()->GetArchetype("Jaxium Archetype"));
			jaxium->AddComponent(new Crystal(Crystal::CrystalType::Jaxium, jaxiumDropAmount));
			jaxium->GetComponent<Transform>()->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID]));

			Space::GetLayer(0)->GetGameObjectManager()->Add(*jaxium);
		}
		if (neoridiumDropAmount > 0) {
			//Create a neoridium crystal on the node.
			GameObject* neoridium = new GameObject(*Space::GetLayer(0)->GetGameObjectManager()->GetArchetype("Neoridium Archetype"));
			neoridium->AddComponent(new Crystal(Crystal::CrystalType::Neoridium, neoridiumDropAmount));
			neoridium->GetComponent<Transform>()->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID]));

			Space::GetLayer(0)->GetGameObjectManager()->Add(*neoridium);
		}
	}
}

void Building::OpenMenu(Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
}

float Building::Variance(float value, float variance) {
	float change = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / variance));

	if (rand() % 2) return value + value * change;
	return value - value * change;
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
