#pragma once

#include "Button.h"

class RestartButton : public Button {
public:
	RestartButton();
	Component *Clone() const;
	void ClickEffect(float dt);
};