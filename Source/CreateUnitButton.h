#pragma once

#include "ArmyButton.h"

class CreateUnitButton : public ArmyButton {
public:
	CreateUnitButton();
	void Load(rapidjson::Value &obj);
	Component *Clone() const;

	void ClickEffect(float dt);

private:
	string unitName;
};
