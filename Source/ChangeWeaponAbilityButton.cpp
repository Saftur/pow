#include "stdafx.h"
#include "ChangeWeaponAbilityButton.h"

using std::to_string;

#include "Rendertext.h"
#include "CustomPointCounter.h"

ChangeWeaponAbilityButton::ChangeWeaponAbilityButton() : Button("ChangeWeaponAbilityButton") {
}

Component * ChangeWeaponAbilityButton::Clone() const {
	return new ChangeWeaponAbilityButton(*this);
}

void ChangeWeaponAbilityButton::ClickEffect(float dt) {
	if (*statPtr < (int)pointCosts.size())
		pointCounter->GivePoints(pointCosts[*statPtr]);
	for (unsigned i = 0; i < statTexts.size(); i++) {
		*statPtr += amount;
		if (*statPtr < 0) *statPtr = statTexts.size()-1;
		else if (*statPtr >= (int)statTexts.size()) *statPtr = 0;
		if (*statPtr >= (int)pointCosts.size() || pointCounter->TakePoints(pointCosts[*statPtr]))
			break;
	}
	UpdateStatTextComp();
}

void ChangeWeaponAbilityButton::SetStatPtr(int * statPtr) {
	this->statPtr = statPtr;
	UpdateStatTextComp();
}

void ChangeWeaponAbilityButton::SetStatAmount(int amount) {
	this->amount = amount;
}

void ChangeWeaponAbilityButton::SetStatTextComp(Text * statTextComp) {
	this->statTextComp = statTextComp;
	UpdateStatTextComp();
}

void ChangeWeaponAbilityButton::SetStatTexts(vector<string> statTexts) {
	this->statTexts = statTexts;
	UpdateStatTextComp();
}

void ChangeWeaponAbilityButton::SetPointCounter(CustomPointCounter * pointCounter) {
	if (this->pointCounter && statPtr && *statPtr < (int)pointCosts.size())
		this->pointCounter->GivePoints(pointCosts[*statPtr]);
	this->pointCounter = pointCounter;
	if (pointCounter && statPtr && *statPtr < (int)pointCosts.size()) {
		if (!pointCounter->TakePoints(pointCosts[*statPtr])) {
			*statPtr = 0;
		}
	}
}

void ChangeWeaponAbilityButton::SetStat(int * statPtr, int amount, Text * statTextComp, const vector<string> &statTexts, const vector<int> &pointCosts, CustomPointCounter *pointCounter) {
	this->statPtr = statPtr;
	this->amount = amount;
	this->statTextComp = statTextComp;
	this->statTexts = statTexts;
	this->pointCosts = pointCosts;
	SetPointCounter(pointCounter);
	UpdateStatTextComp();
}

void ChangeWeaponAbilityButton::UpdateStatTextComp() {
	statTextComp->SetText(statTexts[*statPtr]);
}
