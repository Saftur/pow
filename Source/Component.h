//------------------------------------------------------------------------------
//
// File Name:	Component.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Serializable.h"

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Camera Camera;
typedef class GameObject GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface (function prototypes) declared in the header.
class Component : public Serializable
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	Component(const char* name);

	// Destructor - virtual so that correct destructor is called.
	virtual ~Component();

	// Clones the component, returning a dynamically allocated copy.
	virtual Component* Clone() const = 0;

	virtual void InstanceInit();

	// Logic update for this component.
	virtual void Update(float dt);

	// Rendering phase for this component.
	virtual void Draw(Camera *cam) const;

	virtual void OnDestroy();

	// Retrieve component's parent object (if any).
	GameObject* GetParent() const;

	// Set component's parent object/owner.
	virtual void SetParent(GameObject* object);

	// Get the name of this component's type.
	const char* GetName() const;

protected:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Object that owns this component
	GameObject* parent;

	// Name of component type (e.g. Transform, Sprite, etc.)
	const char* name;
};

//------------------------------------------------------------------------------
