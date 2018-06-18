#include "stdafx.h"
#include "LoadArmyButton.h"

using std::to_string;

#include "GameObjectManager.h"
#include "GameObject.h"
#include "TextBox.h"
#include "ChangeStatButton.h"
#include "ChangeWeaponAbilityButton.h"
#include "CustomPointCounter.h"
#include "CostDisplay.h"

LoadArmyButton::LoadArmyButton() : Button("LoadArmyButton") {
}

Component * LoadArmyButton::Clone() const {
	return new LoadArmyButton(*this);
}

void LoadArmyButton::ClickEffect(float dt) {
	GameObjectManager *gom = GetParent()->GetGameObjectManager();
	ArmyTraits *armyTraits = gom->GetObjectByName("MainObject")->GetComponent<ArmyTraits>();
	string armyFileName = gom->GetObjectByName("ArmyName")->GetComponent<TextBox>()->GetText();

	armyTraits->SetArmyFile(armyFileName);
	armyTraits->LoadArmyFile();

	unsigned unitNum = 1;
	for (BehaviorUnit::Traits &unit : armyTraits->unitTypes) {
		// Get unit name object
		GameObject *name = gom->GetObjectByName((string("Name")+to_string(unitNum)).c_str());
		// Update to army file value
		name->GetComponent<TextBox>()->SetText(unit.name);

		// Get unit point counter
		CustomPointCounter *pointCounter = gom->GetObjectByName((string("PointCounter")+to_string(unitNum)).c_str())->GetComponent<CustomPointCounter>();
		// Reset point count
		pointCounter->SetPoints(25);

		// Get unit cost display
		CostDisplay *costDisplay = gom->GetObjectByName((string("CostDisplay")+to_string(unitNum)).c_str())->GetComponent<CostDisplay>();
		// Set display unit
		costDisplay->SetUnit(&unit);

#define NUM_STATS 3
		static string stats[NUM_STATS] = {"Strength", "Agility", "Defense"};
		int *statPtrs[NUM_STATS] = {&unit.strength, &unit.agility, &unit.defense};

		for (unsigned i = 0; i < NUM_STATS; i++) {
			string stat = stats[i];
			int *statPtr = statPtrs[i];
			string statText = stat+":";

			// Take points from counter
			if (!pointCounter->TakePoints(*statPtr)) {
				// If not enough points, set to max points left
				*statPtr = pointCounter->GetPoints();
				// and set point counter to 0
				pointCounter->SetPoints(0);
			}

			// Get unit stat object
			GameObject *statObj = gom->GetObjectByName((stat+to_string(unitNum)).c_str());
			// Get stat text component
			Text *statTextComp = statObj->GetComponent<Text>();
			// Update to army file value
			statTextComp->SetText(string(stat)+to_string(*statPtr));

			// Get unit stat up object
			GameObject *statUp = gom->GetObjectByName((stat+"Up"+to_string(unitNum)).c_str());
			// Update to army file value
			statUp->GetComponent<ChangeStatButton>()->SetStatPtr(statPtr);

			// Get unit stat down object
			GameObject *statDown = gom->GetObjectByName((stat+"Down"+to_string(unitNum)).c_str());
			// Update to army file value
			statDown->GetComponent<ChangeStatButton>()->SetStatPtr(statPtr);
		}
#undef NUM_STATS

		// Take points from counter
		if (unit.ability != cAbilityNone && !pointCounter->TakePoints(5)) {
			// If not enough points, set to no ability
			unit.ability = cAbilityNone;
		}

		// Get unit ability object
		GameObject *abilityObj = gom->GetObjectByName(("AbilityV"+to_string(unitNum)).c_str());
		// Get ability text component
		Text *abilityTextComp = abilityObj->GetComponent<Text>();
		// Update to army file value
		abilityTextComp->SetText(string("AbilityV")+to_string(unit.ability));

		// Get unit next ability object
		GameObject *abilityNext = gom->GetObjectByName((string("AbilityNext")+to_string(unitNum)).c_str());
		// Update to army file value
		abilityNext->GetComponent<ChangeWeaponAbilityButton>()->SetStatPtr(&unit.ability);

		// Get unit prev ability object
		GameObject *abilityPrev = gom->GetObjectByName((string("AbilityPrev")+to_string(unitNum)).c_str());
		// Update to army file value
		abilityPrev->GetComponent<ChangeWeaponAbilityButton>()->SetStatPtr(&unit.ability);

		// Get unit weapon object
		GameObject *weaponObj = gom->GetObjectByName(("WeaponV"+to_string(unitNum)).c_str());
		// Get weapon text component
		Text *weaponTextComp = weaponObj->GetComponent<Text>();
		// Update to army file value
		weaponTextComp->SetText(string("WeaponV")+to_string(unit.weapon));

		// Get unit next weapon object
		GameObject *weaponNext = gom->GetObjectByName((string("WeaponNext")+to_string(unitNum)).c_str());
		// Update to army file value
		weaponNext->GetComponent<ChangeWeaponAbilityButton>()->SetStatPtr(&unit.weapon);

		// Get unit prev weapon object
		GameObject *weaponPrev = gom->GetObjectByName((string("WeaponPrev")+to_string(unitNum)).c_str());
		// Update to army file value
		weaponPrev->GetComponent<ChangeWeaponAbilityButton>()->SetStatPtr(&unit.weapon);

		unitNum++;
	}
}
