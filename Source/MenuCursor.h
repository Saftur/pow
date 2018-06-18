//------------------------------------------------------------------------------
//
// File Name:	Cursor.h
// Author(s):	Arthur Bouvier
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
#include "Gamepad.h"

typedef class Transform Transform;
typedef class GameObject GameObject;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in Cursor.h
class MenuCursor : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Cursor) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	MenuCursor();

	void Load(rapidjson::Value &obj);

	void InstanceInit();

	void Update(float dt);

	void SetGamepad(Gamepad *gp);

private:
	// Clone an advanced behavior and return a pointer to the cloned object.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	//   parent = A reference to the parent object (the object that owns this component).
	// Returns:
	//   A pointer to an advanced behavior.
	Component* Clone() const;

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	Gamepad *gamepad;
	Transform *transform;

	GameObject *selectedButton;
	float coneAngle; // Half of angle of the "cone" to use when finding buttons in specified direction
	float offScale; // How much to scale distance by depending on angle from direction angle
					// (basically, count as farther if not straight in given direction)
};

//------------------------------------------------------------------------------
