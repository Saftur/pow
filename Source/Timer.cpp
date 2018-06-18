//------------------------------------------------------------------------------
//
// File Name:	ComponentTimer.cpp
// Author(s):	Mark Culp
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
#include "Timer.h"
#include "LevelManager.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (ComponentTimer) behavior component.
// Params:
//  parent = The object that owns this behavior.
Timer::Timer() : Component("Timer")
{
	timer = 0;
}

void Timer::Update(float dt) 
{
	if (timer < duration)
	{
		timer += dt;
	}
	else
	{
		GetParent()->GetLevelManager()->SetNextLevel(path);
	}
}

void Timer::Load(rapidjson::Value& obj)
{
	path = obj["Level"].GetString();
	duration = obj["Duration"].GetFloat();
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* Timer::Clone() const
{
	return new Timer(*this);
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
