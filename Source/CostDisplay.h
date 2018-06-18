#pragma once

#include "Component.h"
#include "BehaviorUnit.h"

typedef class Text Text;

class CostDisplay : public Component {
public:
	CostDisplay();

	void InstanceInit();

	Component *Clone() const;

	void Update(float dt);

	void SetUnit(BehaviorUnit::Traits *unit);

private:
	BehaviorUnit::Traits *unit;

	Text *textComp;
};
