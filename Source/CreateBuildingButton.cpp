#include "stdafx.h"
#include "CreateBuildingButton.h"

#include <AEEngine.h>
#include "Space.h"

#include "BuildingJaxiumMine.h"
#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingTeleporter.h"

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
	if (army->LegalSpawn(mapPos->gridPos())) {
		GameObject *newBuilding = new GameObject(*buildingArchetype);
		Building *building = newBuilding->GetChildComponent<Building>();
		building->SetPos(mapPos->gridPos());
		building->SetArmy(army);
		building->Buy();
		Space::GetLayer(0)->GetGameObjectManager()->Add(*newBuilding);
	}
}
