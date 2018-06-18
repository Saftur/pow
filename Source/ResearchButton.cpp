#include "stdafx.h"
#include "ResearchButton.h"

ResearchButton::ResearchButton() : ArmyButton("ResearchButton") {
}

void ResearchButton::Load(rapidjson::Value & obj) {
	ArmyButton::Load(obj);
	rapidjson::Value *tmp;

	if (obj.HasMember("ResearchType") && (tmp = &obj["ResearchType"])->IsString()) {
		string type = tmp->GetString();
		if (type == "Turret") researchType = BuildingResearchCenter::Research::Turret;
		else if (type == "Teleporter") researchType = BuildingResearchCenter::Research::Teleporter;
	}
}

Component * ResearchButton::Clone() const {
	return new ResearchButton(*this);
}

void ResearchButton::Update(float dt)
{
	SetActive(BuildingResearchCenter::CanUnlock(army->GetSide(), researchType));
}

void ResearchButton::ClickEffect(float dt) {
	BuildingResearchCenter::Unlock(army->GetSide(), (Building::BuildingType)researchType);

	army->UpdateNeoridiumFundsText();
}
