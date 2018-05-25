#include "stdafx.h"
#include "CreateUnitButton.h"
#include <algorithm>

CreateUnitButton::CreateUnitButton() : ArmyButton("CreateUnitButton") {
}

void CreateUnitButton::Load(rapidjson::Value & obj) {
	ArmyButton::Load(obj);
	rapidjson::Value *tmp;

	if (obj.HasMember("UnitName") && (tmp = &obj["UnitName"])->IsString()) {
		unitName = tmp->GetString();
	}
}

Component * CreateUnitButton::Clone() const {
	return new CreateUnitButton(*this);
}

void CreateUnitButton::ClickEffect(float dt) {
	vector<GridManager::Node*> nearbyNodes = GridManager::GetInstance().GetNeighbors(mapPos);
	for (unsigned i = 0; i < nearbyNodes.size(); i++) {
		if (!army->LegalSpawn(nearbyNodes[i]->gridPos())) nearbyNodes[i] = nullptr;
	}
	nearbyNodes.erase(std::remove(nearbyNodes.begin(), nearbyNodes.end(), nullptr), nearbyNodes.end());
	if (nearbyNodes.size() == 0) return;

	int nodeID = rand() % nearbyNodes.size();

	army->CreateUnit(unitName.c_str(), nearbyNodes[nodeID]->gridPos());
}
