#pragma once

#include "Button.h"

class SaveArmyButton : public Button {
public:
	SaveArmyButton();

	Component *Clone() const;

	void ClickEffect(float dt);

private:

};
