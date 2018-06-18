#include "stdafx.h"
#include "CustomizationMain.h"

#include "GameObjectManager.h"
#include "GameObject.h"
#include "Button.h"

CustomizationMain::CustomizationMain() : Component("CustomizationMain") {
}

Component * CustomizationMain::Clone() const {
	return new CustomizationMain(*this);
}

void CustomizationMain::Update(float dt) {
	if (!done) {
		Button *newUnitButton = GetParent()->GetGameObjectManager()->GetObjectByName("NewUnitButton")->GetChildComponent<Button>();
		Button *loadArmyButton = GetParent()->GetGameObjectManager()->GetObjectByName("LoadArmyButton")->GetChildComponent<Button>();

		for (unsigned i = 0; i < 4; i++)
			newUnitButton->Click(0);

		loadArmyButton->Click(0);

		done = true;
	}
}
