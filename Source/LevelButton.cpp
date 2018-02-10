//------------------------------------------------------------------------------
//
// File Name:	LevelButton.cpp
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h" // AEGfxGetWinMaxX, MinX, MaxY, MinY
#include "GameStateManager.h"
#include "LevelButton.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Asteroid) behavior component.
LevelButton::LevelButton(int level) : targetLevel(level) {

}

void LevelButton::SetLevel(int level) {
	targetLevel = level;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Clone an advanced behavior and return a pointer to the cloned object.
// Returns:
//   A pointer to a dynamically allocated clone of the advanced behavior.
Component* LevelButton::Clone() const {
	return new LevelButton(*this);
}

//What happens when a button is clicked.
void LevelButton::ClickEffect(float dt) {
	UNREFERENCED_PARAMETER(dt);
	GameStateManager::GetInstance().SetNextState(targetLevel);
}
