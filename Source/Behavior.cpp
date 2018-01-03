//------------------------------------------------------------------------------
//
// File Name:	Behavior.cpp
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
#include "Behavior.h"
#include "GameObject.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a behavior, initialize variables to sane values.
Behavior::Behavior(GameObject& parent)
	: parent(parent), timer(0) {}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to a basic or advanced behavior.
Behavior* Behavior::Clone(GameObject& theParent) const
{
	if (clone)
		return clone(*this, theParent);
	else 
		return new Behavior(*this, theParent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void Behavior::Destroy(Behavior* behavior)
{
	if (behavior && behavior->destroy)
		behavior->destroy(*behavior);
	else
		delete(behavior);
}

// Allocate a clone of an existing behavior.
// (Hint: Perform a shallow copy of the member variables, then change the 'parent' pointer.)
// Params:
//	 other = Reference to the component to be cloned.
//   parent = Pointer to object that owns this component.
Behavior::Behavior(const Behavior& other, GameObject& theParent)
	: clone(other.clone), destroy(other.destroy), timer(other.timer), stateCurr(other.stateCurr), stateNext(other.stateNext), onInit(other.onInit), onUpdate(other.onUpdate), onExit(other.onExit), parent(theParent) {}

// Update the behavior component.
// (Hint: Refer to the Word document for detailed instructions regarding this function.)
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Behavior::Update(float dt)
{
	if (stateCurr != stateNext)
	{
		if (onExit)
			onExit(*this);

		stateCurr = stateNext;

		if (onInit)
			onInit(*this);
	}

	if (onUpdate)
		onUpdate(*this, dt);
}

// Destructor for Behavior class.
// No one is allowed to destroy a behavior except a behavior or friend class.
Behavior::~Behavior()
{

}

//------------------------------------------------------------------------------
