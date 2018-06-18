#pragma once

#include <string>
using std::string;

#include "ArmyButton.h"

typedef class GameObject GameObject;

class CreateBuildingButton : public ArmyButton {
public:
	CreateBuildingButton();
	void Load(rapidjson::Value &obj);
	Component *Clone() const;
	void InstanceInit();
	void Update(float dt);

	void ClickEffect(float dt);

private:
	string archetypeName;
	GameObject *buildingArchetype;
	Building::BuildingType buildingType;
};