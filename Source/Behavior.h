//------------------------------------------------------------------------------
//
// File Name:	Behavior.h
// Author(s):	Doug Schilling (dschilling)
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
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// NOTE: You are not allowed to change the contents of this structure, as it is
// part of the public interface.
class Behavior : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a behavior, initialize variables to sane values.
	Behavior();

	// Destructor for Behavior class.
	// Marked virtual so that correct destructor is called.
	virtual ~Behavior();

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a basic or advanced behavior.
	virtual Component* Clone() const = 0;

	// Update the behavior component.
	// (Hint: Refer to the Word document for detailed instructions regarding this function.)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	// Executes when entering the current state of the behavior component.
	virtual void OnEnter();
	// Executes every update for the current state of the behavior component.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	virtual void OnUpdate(float dt);
	// Executes upon exiting the current state of the behavior component.
	virtual void OnExit();

	// Gets the current state of the behavior state machine.
	int GetCurrentState() const;
	// Sets the next state of the behavior state machine.
	// Params:
	//	 state = The next state of the state machine.
	void SetNextState(int state);

	// Return the value of the behavior timer.
	float GetTimer() const;
	// Sets the behavior timer
	// Params:
	//	 timer = The new value for the timer (in seconds).
	void SetTimer(float time);

protected:
	// Sets the current state of the behavior state machine. 
	// DO NOT use except in special circumstances, e.g. initial behavior setup.
	// Params:
	//	 state = The new current state of the state machine.
	void SetCurrentState(int state);

	const int cBehaviorInvalid = -1;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Behavior Finite-State Machine (FSM) state variables.
	int stateCurr;
	int stateNext;

	// Additional variables shared by all behaviors.
	// NOTE: Variables that are unique to a specific behavior should not be placed here.

	// Generic timer - may be used as a life timer or a weapon cooldown timer.
	// (Default = 0, means infinite amount of time remaining or weapon can be fired.)
	float	timer;
};
//------------------------------------------------------------------------------
