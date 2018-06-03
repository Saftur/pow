#include "stdafx.h"
#include "SaveArmyButton.h"

using std::to_string;

#include "GameObjectManager.h"
#include "GameObject.h"
#include "TextBox.h"

SaveArmyButton::SaveArmyButton() : Button("SaveArmyButton") {
}

Component * SaveArmyButton::Clone() const {
	return new SaveArmyButton(*this);
}

void SaveArmyButton::ClickEffect(float dt) {
	GameObjectManager *gom = GetParent()->GetGameObjectManager();
	ArmyTraits *armyTraits = gom->GetObjectByName("MainObject")->GetComponent<ArmyTraits>();
	string armyFileName = gom->GetObjectByName("ArmyName")->GetComponent<TextBox>()->GetText();

	unsigned unitNum = 1;
	for (BehaviorUnit::Traits &unit : armyTraits->unitTypes) {
		// Get unit name object
		GameObject *name = gom->GetObjectByName((string("Name")+to_string(unitNum)).c_str());
		// Update army traits value
		unit.name = name->GetComponent<TextBox>()->GetText();
		unitNum++;
	}

	armyTraits->SetArmyFile(armyFileName);
	armyTraits->SaveArmyFile();
}
