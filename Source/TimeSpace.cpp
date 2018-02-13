//------------------------------------------------------------------------------
// File Name:	TimeSpace.cpp
// Author(s):	Brand Knutson
// Project:		PoW
// Course:		GAM150
//
// Copyright © 2018 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "TimeSpace.h"
#include "AEEngine.h"

TimeSpace::TimeSpace() : Component("TimeSpace") {
}

Component * TimeSpace::Clone() const {
	return new TimeSpace(*this);
}

void TimeSpace::Update(float dt) {
	UNREFERENCED_PARAMETER(dt);
	//If escape is pressed, pause the game
	if (AEInputCheckTriggered(VK_ESCAPE)) TogglePaused();
}

bool TimeSpace::IsPaused() {
	return paused;
}

void TimeSpace::TogglePaused() {
	paused = !paused;
}
