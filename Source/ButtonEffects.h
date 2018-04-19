#pragma once

#include "Button.h"

class ButtonEffects {
public:
	static void List();

private:
	// Define game-side button effects here
	static void KillAllUnits(Button &button, float dt, int count = 0, ...);
};