#pragma once

#include "Component.h"

class CustomizationMain : public Component {
public:
	CustomizationMain();

	Component *Clone() const;

	void Update(float dt);

private:
	bool done;
};
