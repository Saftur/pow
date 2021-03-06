//------------------------------------------------------------------------------
//
// File Name:	Building.cpp
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

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
#include "Health.h"

#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingCommandPost.h"
#include "BuildingTeleporter.h"
#include <algorithm>
#include "SoundManager.h"
#include "PopupMenu.h"

map<BehaviorArmy::Side, bool[Building::BuildingType::BuildingCount]> Building::buildings;

map<Building::BuildingType, float> Building::buildingCost;
vector<GameObject*> Building::allBuildings;

Building::Building(BehaviorArmy::Side side, BuildingType type, SpecialtyType specialtyType, float buildTime, Vector2D pos, float jaxiumDropAmount, 
	float neoridiumDropAmount) : Component("Building"), buildingType(type), specialtyType(specialtyType), buildTime(buildTime), mapPos(pos), side(side), jaxiumDropAmount(jaxiumDropAmount), neoridiumDropAmount(neoridiumDropAmount)
{
	buildTimeRemaining = buildTime; //Set the delay until the building is finnished being built.
	originalScale = { 0, 0 }; //Default to 0 so we can check if we set it yet or not on update when we actually have the transform component attatched.
}

Building::~Building()
{
	allBuildings.erase(std::remove(allBuildings.begin(), allBuildings.end(), GetParent()), allBuildings.end());
}

void Building::OnDestroy()
{
	GridManager::GetInstance().SetNode(mapPos, true);

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

void Building::InitializeBuildings(BehaviorArmy::Side side)
{
	srand((unsigned) time(NULL));
	BuildingResearchCenter::InitializeResearchCost();

	for (int i = 0; i < BuildingCount; i++) Lock(side, (BuildingType)i); //Lock all of the buildings.
	Unlock(side, JaxiumMine);
	Unlock(side, ResearchCenter);

	BuildingNeoridiumMine::neoridium[side] = 0.0f; //Intitialize the amount of Neoridium each player has to 0.

	//Inititialize the cost of buildings.
	buildingCost[JaxiumMine] = 250.0f;
	buildingCost[NeoridiumMine] = 200.0f;
	buildingCost[ResearchCenter] = 250.0f;
	buildingCost[Spaceport] = 350.0f;
	buildingCost[VehicleDepot] = 300.0f;
	buildingCost[Turret] = 200.0f;
	buildingCost[Teleporter] = 150.0f;
	buildingCost[CommandPost] = -0.0f;

	//Initialize the number of teleporters each army has to 0.
	BuildingTeleporter::teleporters[side] = 0;
}

void Building::Update(float dt)
{
	//Destroy this building if the unit building it got destroyed.
	if (builder && builder->IsDestroyed()) {
		builder->GetComponent<BehaviorUnit>()->isBuilding = false;
		builder = nullptr;
		GetParent()->Destroy();
	}

	//If the building hasn't finnished being built yet, don't update it.
	if (buildTimeRemaining > 0) {
		buildTimeRemaining -= dt; //Lower the time until the building is finnished being built.

		//If we havent set the original scale, set it now and then set the scale to 0.
		if (originalScale == Vector2D(0, 0)) {
			if(buildingType != Teleporter) GridManager::GetInstance().SetNode(mapPos, false);
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
		//Make sure that the unit who built this building can move again.
		if (builder) {
			builder->GetComponent<BehaviorUnit>()->isBuilding = false;
			builder = nullptr;
		}

		BuildingUpdate(dt); //Update the building.
	}
}

void Building::OpenMenu()
{
	PopupMenu::CreateMenu(army, PopupMenu::MenuType::SellBuilding, GridManager::GetInstance().GetNode(mapPos));
}

float Building::Variance(float value, float variance) {
	float change = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / variance));

	if (rand() % 2) return value + value * change;
	return value - value * change;
}

void Building::SetArmy(BehaviorArmy * army) {
	this->army = army;
	this->side = army->GetSide();
}

void Building::SetSide(BehaviorArmy::Side side)
{
	this->side = side;
	//Find the army that this building should belong to.
	vector<GameObject*> objs = Space::GetLayer(0)->GetGameObjectManager()->GetObjectsByName("Army");
	for (unsigned i = 0; i < objs.size(); i++) {
		BehaviorArmy* bArmy = objs[i]->GetComponent<BehaviorArmy>();
		if (bArmy->GetSide() == side) {
			army = bArmy;
		}
	}
}

void Building::SetPos(Vector2D pos)
{
	mapPos = pos;
	GetParent()->GetComponent<Transform>()->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(pos));
}

Vector2D Building::GetGridPos() const {
	return mapPos;
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

bool Building::CanBuy(BehaviorArmy *army, BuildingType type) {
	if (!IsUnlocked(army->GetSide(), type))
		return false;
	if (type == Teleporter) {
		return BuildingNeoridiumMine::GetNeoridium(army->GetSide()) >= Building::buildingCost[type];
	}
	else return army->GetFunds() >= Building::buildingCost[type];
}

bool Building::IsUnlocked() {
	return Building::IsUnlocked(side, buildingType);
}

bool Building::CanBuy() {
	return Building::CanBuy(army, buildingType);
}

bool Building::Buy() {
	if (buildingType == Building::Teleporter) {
		army->UpdateNeoridiumFundsText();
		if (BuildingNeoridiumMine::TakeNeoridium(side, Building::buildingCost[buildingType])) return true;
	}
	else if (army->TakeFromFunds(Building::buildingCost[buildingType])) return true; //Throw an error if we can't pay for the building. (This should never happen).
	return false;
}

void Building::Sell()
{
	Health *health = GetParent()->GetComponent<Health>();
	float healthPercent = (float)health->GetHP() / (float)health->GetMaxHP();

	if (buildingType == Building::Teleporter) {
		BuildingNeoridiumMine::AddNeoridium(side, buildingCost[buildingType] * 0.75f * healthPercent);
		army->UpdateNeoridiumFundsText();
	}
	else {
		army->AddToFunds(buildingCost[buildingType] * 0.75f * healthPercent);
	}

	if (GetParent()->GetComponent<BuildingCommandPost>()) health->UpdateHP(-health->GetHP());
	
	ClearDrops();
	GetParent()->Destroy();
}

bool Building::IsBuilt() {
	return buildTimeRemaining <= 0;
}

void Building::ClearDrops()
{
	neoridiumDropAmount = 0;
	jaxiumDropAmount = 0;
}
