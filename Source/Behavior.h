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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef class Behavior Behavior;

typedef Behavior*(*BehaviorFunctionPtrGo)(const Behavior& behavior, GameObject& parent);
typedef void(*BehaviorFunctionPtr)(Behavior& behavior);
typedef void(*BehaviorFunctionPtrDt)(Behavior& behavior, float dt);

// NOTE: You are not allowed to change the contents of this structure, as it is
// part of the public interface.
class Behavior
{
public:
	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	friend class BehaviorAsteroid;
	friend class BehaviorBullet;
	friend class BehaviorSpaceship;
	friend class BehaviorLevelButton;
	friend class BehaviorButtonStub;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a behavior, initialize variables to sane values.
	// Params:
	//  parent = A reference to the parent object (the object that owns this component).
	Behavior(GameObject& parent);

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Params:
	//   parent = A reference to the parent object (the object that owns this component).
	// Returns:
	//   A pointer to a basic or advanced behavior.
	Behavior* Clone(GameObject& parent) const;

	// Destroy an advanced behavior.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	static void Destroy(Behavior* behavior);

	// Update the behavior component.
	// (Hint: Refer to the Word document for detailed instructions regarding this function.)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	// Remove copy constructor
	Behavior(const Behavior& behavior) = delete;


private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Copy a basic behavior object.
	// (Hint: Perform a shallow copy of the member variables, then change the 'parent' pointer.)
	// Params:
	//	 other = Reference to the component to be cloned.
	//   parent = Reference to object that owns this component.
	Behavior(const Behavior& other, GameObject& parent);

	// Destructor for Behavior class.
	// No one is allowed to destroy a behavior except a behavior or friend class.
	~Behavior();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Reference to the behavior's parent game object.
	GameObject& parent;

	// Behavior Finite-State Machine (FSM) state variables.
	int stateCurr;
	int stateNext;

	// Behavior Finite-State Machine (FSM) function pointers.
	BehaviorFunctionPtr		onInit;
	BehaviorFunctionPtrDt	onUpdate;
	BehaviorFunctionPtr		onExit;

	// Advanced behavior cloning and destruction function pointers.
	BehaviorFunctionPtrGo	clone;
	BehaviorFunctionPtr		destroy;

	// Additional variables shared by all behaviors.
	// NOTE: Variables that are unique to a specific behavior should not be placed here.

	// Generic timer - may be used as a life timer or a weapon cooldown timer.
	// (Default = 0, means infinite amount of time remaining or weapon can be fired.)
	float	timer;
};
//------------------------------------------------------------------------------
