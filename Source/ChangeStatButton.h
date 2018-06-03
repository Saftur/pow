#pragma once

#include <string>
using std::string;

#include "Button.h"

typedef class Text Text;
typedef class CustomPointCounter CustomPointCounter;

class ChangeStatButton : public Button {
public:
	ChangeStatButton();

	Component *Clone() const;

	void ClickEffect(float dt);

	void SetStatPtr(int *statPtr);
	void SetStatAmount(int amount);
	void SetStatTextComp(Text *statTextComp);
	void SetStatText(string statText);
	void SetPointCounter(CustomPointCounter *pointCounter);
	void SetStat(int *statPtr, int amount, Text *statTextComp, string statText, CustomPointCounter *pointCounter);

private:
	void UpdateStatTextComp();

	int *statPtr;
	int amount;

	Text *statTextComp;
	string statText;

	CustomPointCounter *pointCounter;
};
