#pragma once

#include "ArmyButton.h"

class TurretUpgradeButton : public ArmyButton {
public:
	TurretUpgradeButton();
	void Load(rapidjson::Value &obj);
	Component *Clone() const;

	void Update(float dt);

	void ClickEffect(float dt);

private:
	string upgradeType;
	Node *node; //Position of turret.
};
