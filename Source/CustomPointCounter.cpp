#include "stdafx.h"
#include "CustomPointCounter.h"

#include <string>
using std::to_string;

#include "GameObject.h"
#include "RenderText.h"

CustomPointCounter::CustomPointCounter() : Component("CustomPointCounter") {
}

void CustomPointCounter::InstanceInit() {
	textComp = GetParent()->GetComponent<Text>();
}

Component * CustomPointCounter::Clone() const {
	return new CustomPointCounter(*this);
}

bool CustomPointCounter::TakePoints(int amount) {
	if (amount > points)
		return false;
	points -= amount;
	UpdateCounterText();
	return true;
}

void CustomPointCounter::GivePoints(int amount) {
	points += amount;
	UpdateCounterText();
}

void CustomPointCounter::SetPoints(int amount) {
	points = amount;
	UpdateCounterText();
}

int CustomPointCounter::GetPoints() const {
	return points;
}

void CustomPointCounter::UpdateCounterText() {
	if (textComp)
		textComp->SetText(string("Points:")+to_string(points));
}
