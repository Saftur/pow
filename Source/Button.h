//------------------------------------------------------------------------------
//
// File Name:	Button.h
// Author(s):	Brand Knutson & Arthur Bouvier
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
#include "Rendertext.h"
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <string>
using std::string;

#include "BehaviorArmy.h"
#include "Building.h"
#include "BuildingResearchCenter.h"

typedef class Vector2D Vector2D;
typedef class GameObject GameObject;
typedef class Transform Transform;
typedef struct AEGfxTexture AEGfxTexture;
typedef class Component Component;
typedef class SpriteSource SpriteSource;
typedef class Text Text;
typedef struct Color color;

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
	Button(const char *name);

	//Check if the button is being pressed
	void Update(float dt);
	
	void Click(float dt);
	virtual void ClickEffect(float dt) = 0;

	void SetActive(bool isActive);
	bool GetActive() const;

private:
	bool active = true; //Is this button active.

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
