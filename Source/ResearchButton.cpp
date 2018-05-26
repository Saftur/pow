#include "stdafx.h"
#include "ResearchButton.h"

ResearchButton::ResearchButton() : ArmyButton("ResearchButton") {
}

void ResearchButton::Load(rapidjson::Value & obj) {
	ArmyButton::Load(obj);
	rapidjson::Value *tmp;

	if (obj.HasMember("ResearchType") && (tmp = &obj["ResearchType"])->IsString()) {
		researchType = tmp->GetString();
	}
}

Component * ResearchButton::Clone() const {
	return new ResearchButton(*this);
}

void ResearchButton::ClickEffect(float dt) {
	if (researchType == "Spaceport") {
		BuildingResearchCenter::Unlock(army->GetSide(), Building::BuildingType::Spaceport);
	}
	else if (researchType == "VehicleDepot") {
		BuildingResearchCenter::Unlock(army->GetSide(), Building::BuildingType::VehicleDepot);
	}
	else if (researchType == "Turret") {
		BuildingResearchCenter::Unlock(army->GetSide(), Building::BuildingType::Turret);
	}
	else if (researchType == "Teleporter") {
		BuildingResearchCenter::Unlock(army->GetSide(), Building::BuildingType::Teleporter);
	}

	army->UpdateNeoridiumFundsText();
}
