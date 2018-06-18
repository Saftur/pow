#pragma once

#include "Button.h"

class LoadArmyButton : public Button {
public:
	LoadArmyButton();

	Component *Clone() const;

	void ClickEffect(float dt);

private:

};
