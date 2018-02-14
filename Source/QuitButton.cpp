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
#include "QuitButton.h"
#include "LevelManager.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Asteroid) behavior component.
QuitButton::QuitButton() {

}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Clone an advanced behavior and return a pointer to the cloned object.
// Returns:
//   A pointer to a dynamically allocated clone of the advanced behavior.
Component* QuitButton::Clone() const {
	return new QuitButton(*this);
}

//What happens when a button is clicked.
void QuitButton::ClickEffect(float dt) {
	UNREFERENCED_PARAMETER(dt);
	LevelManager::GetInstance().Quit();
}
