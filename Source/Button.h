//------------------------------------------------------------------------------
//
// File Name:	Button.h
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

#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorAsteroid.h
class Button : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new Button component.
	Button();

	template <typename T>
	static GameObject* CreateButton(const char* objName, Vector2D pos = { 0.0f, 0.0f }, Vector2D scale = { 100.0f, 50.0f }, const char* spritePath = nullptr);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a dynamically allocated clone of the advanced behavior.
	virtual Component* Clone() const = 0;

	//Check if the button is being pressed
	void Update(float dt);

	//Update anything on the button that needs updating.
	virtual void OnUpdate(float dt);

	//What happens when a button is clicked.
	virtual void ClickEffect(float dt) = 0;
};

//------------------------------------------------------------------------------
