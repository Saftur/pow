//------------------------------------------------------------------------------
// File Name:	LevelTimer.h
// Author(s):	Brand Knutson
// Project:		Push of War
// Course:		Gam150
//
// Copyright © 2017 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#pragma once

#include "Rendertext.h"

class LevelTimer {
public:
	LevelTimer(Vector2D pos, Vector2D scale);
	void Update(float dt);

private:
	float time = 0.0f;
	Text text;
};