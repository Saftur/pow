#pragma once

#include "ArmyButton.h"

class SellBuildingButton : public ArmyButton {
public:
	SellBuildingButton();
	void Load(rapidjson::Value &obj);
	Component *Clone() const;

	void ClickEffect(float dt);

private:
	string upgradeType;
	Node *node; //Position of building.
};
