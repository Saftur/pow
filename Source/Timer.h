//------------------------------------------------------------------------------
//
// File Name:	ComponentTimer.h
// Author(s):	Mark Culp
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

// An example of the class to be defined in ComponentTimer.h
class Timer : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (ComponentTimer) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	Timer();

	void Update(float dt);
	void Load(rapidjson::Value& obj);

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
	std::string path;
	float timer, duration;
};

//------------------------------------------------------------------------------
