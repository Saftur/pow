#pragma once

#include "Button.h"

class QuitButton : public Button {
public:
	QuitButton();
	Component *Clone() const;
	void ClickEffect(float dt);
};
