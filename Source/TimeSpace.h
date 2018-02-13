//------------------------------------------------------------------------------
// File Name:	TimeSpace.h
// Author(s):	Brand Knutson
// Project:		PoW
// Course:		GAM150
//
// Copyright © 2018 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface (function prototypes) declared in the header.
class TimeSpace : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	TimeSpace();

	// Clones the component, returning a dynamically allocated copy.
	Component* Clone() const;

	// Logic update for this component.
	void Update(float dt);

	//Is the game paused?
	bool IsPaused();

	//Toggle paused.
	void TogglePaused();

private:
	bool paused = false;
};

//------------------------------------------------------------------------------
