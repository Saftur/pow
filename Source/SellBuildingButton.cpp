#include "stdafx.h"
#include "SellBuildingButton.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "GridManager.h"
#include "BuildingTurret.h"

SellBuildingButton::SellBuildingButton() : ArmyButton("SellBuildingButton") {
}

void SellBuildingButton::Load(rapidjson::Value & obj) {
	ArmyButton::Load(obj);

	node = (Node*)GetParent()->GetLevelManager()->GetLoadVar("GridPosition");
}

Component * SellBuildingButton::Clone() const {
	return new SellBuildingButton(*this);
}

void SellBuildingButton::ClickEffect(float dt) {
	GameObject *obj = GridManager::GetInstance().GetOccupant<Building>(node);
	if (obj) obj->GetChildComponent<Building>()->Sell();
}
