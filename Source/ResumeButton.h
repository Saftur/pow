#pragma once

#include "Button.h"

class ResumeButton : public Button {
public:
	ResumeButton();

	Component *Clone() const;

	void ClickEffect(float dt);

private:

};
