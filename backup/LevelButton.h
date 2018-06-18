//------------------------------------------------------------------------------
//
// File Name:	LevelButton.h
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Button.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorAsteroid.h
class LevelButton : public Button
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Asteroid) behavior component.
	LevelButton(const char *level = "");

	//Set the target level.
	void SetLevel(const char *level);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a dynamically allocated clone of the advanced behavior.
	Component* Clone() const;

	//Update anything on the button that needs updating.
	//void OnUpdate(float dt);

	//What happens when a button is clicked.
	void ClickEffect(float dt);

	const char *targetLevel = "";
};

//------------------------------------------------------------------------------
