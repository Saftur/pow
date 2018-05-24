//------------------------------------------------------------------------------
//
// File Name:	BuildingCommandPost.h
// Author(s):	Brand Knutson
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Building.h"
#include "BehaviorArmy.h"
#include "PopupMenu.h"
using std::map;


//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

class BuildingCommandPost : public Building
{
public:

	BuildingCommandPost(BehaviorArmy::Side side, Vector2D pos);
	Component* Clone() const;

	void BuildingUpdate(float dt); //Update the building.

	void OpenMenu(Node *cursorMapPos, Vector2D cursorScreenPos); //Open a menu for the research center.

private:

};

//------------------------------------------------------------------------------
