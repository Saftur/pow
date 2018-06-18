#include "stdafx.h"
#include "CostDisplay.h"

#include <string>
using std::to_string;

#include "GameObject.h"
#include "RenderText.h"

CostDisplay::CostDisplay() : Component("CostDisplay") {
}

void CostDisplay::InstanceInit() {
	textComp = GetParent()->GetComponent<Text>();
}

Component * CostDisplay::Clone() const {
	return new CostDisplay(*this);
}

void CostDisplay::Update(float dt) {
	if (textComp && unit)
		textComp->SetText(string("Cost:")+to_string(unit->GetCost()));
}

void CostDisplay::SetUnit(BehaviorUnit::Traits * unit) {
	this->unit = unit;
}
