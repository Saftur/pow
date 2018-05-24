#pragma once

#include "Button.h"

class RestartButton : public Button {
public:
	RestartButton();
	Component *Clone() const;
	void Click(float dt);
};