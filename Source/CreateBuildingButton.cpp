#include "stdafx.h"
#include "CreateBuildingButton.h"

#include <AEEngine.h>
#include "Space.h"

#include "BuildingJaxiumMine.h"
#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingTeleporter.h"
#include "BuildingTurret.h"

CreateBuildingButton::CreateBuildingButton() : ArmyButton("CreateBuildingButton") {
}

void CreateBuildingButton::Load(rapidjson::Value & obj) {
	ArmyButton::Load(obj);
	rapidjson::Value *tmp;

	if (obj.HasMember("BuildingArchetype") && (tmp = &obj["BuildingArchetype"])->IsString()) {
		archetypeName = tmp->GetString();
		buildingArchetype = Space::GetLayer(0)->GetGameObjectManager()->GetArchetype(archetypeName.c_str());
		if(buildingArchetype) buildingType = buildingArchetype->GetChildComponent<Building>()->buildingType;
	}
}

Component * CreateBuildingButton::Clone() const {
	return new CreateBuildingButton(*this);
}

void CreateBuildingButton::InstanceInit() {
	if (buildingArchetype) {
		SetActive(Building::CanBuy(army, buildingType));
	}
}

void CreateBuildingButton::Update(float dt) {
	if (buildingArchetype) {
		SetActive(Building::CanBuy(army, buildingType));
	}
}

void CreateBuildingButton::ClickEffect(float dt) {
	GameObject* closestUnit = nullptr;
	int closestDistance = 999;

	//Find the closest engineer to this position.
	for (GameObject* unit : BehaviorUnit::allUnits) {
		BehaviorUnit* behavior = unit->GetComponent<BehaviorUnit>();

		if (behavior->traits.ability == Ability::cAbilityEngineer) {
			int distance = GridManager::GetInstance().GetDistanceBetween(mapPos->gridPos(), behavior->GetGridPos());
			if (distance < closestDistance) {
				if (behavior->GetArmy()->GetSide() == army->GetSide()) {
					closestDistance = distance;
					closestUnit = unit;
				}
			}
		}
	}

	//If we found an engineer, send them to build the building.
	if (closestUnit) closestUnit->GetComponent<BehaviorUnit>()->BuildBuilding(buildingArchetype, mapPos);
}
