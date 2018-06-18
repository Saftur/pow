#include "stdafx.h"
#include "ArmyButton.h"

#include "LevelManager.h"

ArmyButton::ArmyButton(const char *name) : Button(name) {
}

void ArmyButton::Load(rapidjson::Value & obj) {
	Button::Load(obj);
	army = (BehaviorArmy*)GetParent()->GetLevelManager()->GetLoadVar("Army");
}

void ArmyButton::SetMapPos(GridManager::Node * mapPos) {
	this->mapPos = mapPos;
}
