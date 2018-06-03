#include "stdafx.h"
#include "ChangeStatButton.h"

using std::to_string;

#include "Rendertext.h"
#include "CustomPointCounter.h"

ChangeStatButton::ChangeStatButton() : Button("ChangeStatButton") {
}

Component * ChangeStatButton::Clone() const {
	return new ChangeStatButton(*this);
}

void ChangeStatButton::ClickEffect(float dt) {
	if (*statPtr+amount >= 0 && pointCounter->TakePoints(amount)) {
		*statPtr += amount;
		UpdateStatTextComp();
	}
}

void ChangeStatButton::SetStatPtr(int * statPtr) {
	this->statPtr = statPtr;
	UpdateStatTextComp();
}

void ChangeStatButton::SetStatAmount(int amount) {
	this->amount = amount;
}

void ChangeStatButton::SetStatTextComp(Text * statTextComp) {
	this->statTextComp = statTextComp;
	UpdateStatTextComp();
}

void ChangeStatButton::SetStatText(string statText) {
	this->statText = statText;
	UpdateStatTextComp();
}

void ChangeStatButton::SetPointCounter(CustomPointCounter * pointCounter) {
	if (this->pointCounter && statPtr)
		this->pointCounter->GivePoints(*statPtr);
	this->pointCounter = pointCounter;
	if (pointCounter && statPtr) {
		if (!pointCounter->TakePoints(*statPtr)) {
			*statPtr = pointCounter->GetPoints();
			pointCounter->SetPoints(0);
		}
	}
}

void ChangeStatButton::SetStat(int * statPtr, int amount, Text * statTextComp, string statText, CustomPointCounter *pointCounter) {
	this->statPtr = statPtr;
	this->amount = amount;
	this->statTextComp = statTextComp;
	this->statText = statText;
	SetPointCounter(pointCounter);
	UpdateStatTextComp();
}

void ChangeStatButton::UpdateStatTextComp() {
	statTextComp->SetText(statText+to_string(*statPtr));
}
