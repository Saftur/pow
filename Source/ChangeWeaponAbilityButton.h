#pragma once

#include <string>
using std::string;

#include "Button.h"

typedef class Text Text;
typedef class CustomPointCounter CustomPointCounter;

class ChangeWeaponAbilityButton : public Button {
public:
	ChangeWeaponAbilityButton();

	Component *Clone() const;

	void ClickEffect(float dt);

	void SetStatPtr(int *statPtr);
	void SetStatAmount(int amount);
	void SetStatTextComp(Text *statTextComp);
	void SetStatTexts(vector<string> statTexts);
	void SetPointCounter(CustomPointCounter *pointCounter);
	void SetStat(int *statPtr, int amount, Text *statTextComp, const vector<string> &statTexts, const vector<int> &pointCosts, CustomPointCounter *pointCounter);

private:
	void UpdateStatTextComp();

	int *statPtr;
	int amount;

	Text *statTextComp;
	vector<string> statTexts;

	vector<int> pointCosts;
	CustomPointCounter *pointCounter;
};
