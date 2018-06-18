#pragma once

#include "Component.h"

typedef class Text Text;

class CustomPointCounter : public Component {
public:
	CustomPointCounter();

	void InstanceInit();

	Component *Clone() const;

	bool TakePoints(int amount);
	void GivePoints(int amount);
	void SetPoints(int amount);
	int GetPoints() const;

private:
	void UpdateCounterText();

	int points;
	Text *textComp;
};
