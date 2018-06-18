//------------------------------------------------------------------------------
//
// File Name:	ComponentHealth.h
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
#include "Vector2D.h"
#include "Sprite.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in ComponentHealth.h
class Health : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (ComponentHealth) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	Health();

	void Load(rapidjson::Value& obj);
	void InstanceInit();

	void OnDestroy();

	void Update(float dt);
	void Draw(Camera* cam) const;

	void Initialize(const int& max);

	int GetHP() const;
	void UpdateHP(const int& amount);
	
	unsigned GetMaxHP() const;
	
	Vector2D GetOffset() const;
	void SetOffset(Vector2D& newOffset);

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
	static unsigned instances;

	static Color background;
	Color currColor;
	int hp, maxHP;
	float hpPercentage;
	static Sprite* bar;
	Vector2D offset, scale;
};

//------------------------------------------------------------------------------
