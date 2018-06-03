#include "stdafx.h"
#include "TurretUpgradeButton.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "GridManager.h"
#include "BuildingTurret.h"

TurretUpgradeButton::TurretUpgradeButton() : ArmyButton("TurretUpgradeButton") {
}

void TurretUpgradeButton::Load(rapidjson::Value & obj) {
	ArmyButton::Load(obj);
	rapidjson::Value *tmp;

	if (obj.HasMember("UpgradeType") && (tmp = &obj["UpgradeType"])->IsString()) {
		upgradeType = tmp->GetString();
		node = (Node*)GetParent()->GetLevelManager()->GetLoadVar("GridPosition");
	}
}

Component * TurretUpgradeButton::Clone() const {
	return new TurretUpgradeButton(*this);
}

void TurretUpgradeButton::Update(float dt)
{
	SetActive(army->GetFunds() >= GridManager::GetInstance().GetOccupant(node)->GetComponent<BuildingTurret>()->upgradeCost);
}

void TurretUpgradeButton::ClickEffect(float dt) {
	GameObject *obj = GridManager::GetInstance().GetOccupant(node);
	BuildingTurret *turret = obj->GetComponent<BuildingTurret>();

	if (army->TakeFromFunds(turret->upgradeCost)) {
		if (upgradeType == "Damage") turret->Upgrade(BuildingTurret::UpgradeType::Damage);
		else if (upgradeType == "Range") turret->Upgrade(BuildingTurret::UpgradeType::Range);
		else if (upgradeType == "AttackSpeed") turret->Upgrade(BuildingTurret::UpgradeType::AttackSpeed);
	}
}
