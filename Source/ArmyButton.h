#pragma once

#include "Button.h"

class ArmyButton : public Button {
public:
	ArmyButton(const char *name);
	void Load(rapidjson::Value &obj);

	void SetMapPos(GridManager::Node *mapPos);

protected:
	GridManager::Node *mapPos;
	BehaviorArmy *army;
};
