#pragma once

#include "ArmyButton.h"

class ResearchButton : public ArmyButton {
public:
	ResearchButton();
	void Load(rapidjson::Value &obj);
	Component *Clone() const;

	void Update(float dt);

	void ClickEffect(float dt);

private:
	BuildingResearchCenter::Research researchType;
};
