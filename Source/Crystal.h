//------------------------------------------------------------------------------
//
// File Name:	Crystal.h
// Author(s):	Brand Knutson
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include "Component.h"


typedef AEGfxVertexList AEGfxVertexList;
typedef AEGfxTexture AEGfxTexture;

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

class Crystal : public Component
{
public:
	enum CrystalType { Jaxium, Neoridium };

	Crystal(CrystalType type, float amount);

	virtual Component* Clone() const;

	void Update(float dt); //Update the Crystal.

	CrystalType type; //Building type.
	float crystalCount; //How many crystals this crystal contains.

private:
};

//------------------------------------------------------------------------------
