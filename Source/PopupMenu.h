//------------------------------------------------------------------------------
//
// File Name:	BuildingMenu.h
// Author(s):	Brand Knutson
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

#include "Gamepad.h"
#include "BehaviorArmy.h"
#include "ControlList.h"
#include <vector>

using std::vector;

//------------------------------------------------------------------------------

typedef class GameObject GameObject;
typedef class Sprite Sprite;

// An example of the class to be defined in BehaviorAsteroid.h
class PopupMenu
{
public:
	enum MenuType { Building, Unit, Research, CommandPost, Turret };

	static void CreateMenu(BehaviorArmy *army, MenuType type, Node *buildingMapPos = nullptr); //Create a menu for the given army.
	static void DestroyMenu(BehaviorArmy::Side side); //Destroy a menu for the given army.
	static bool Exists(BehaviorArmy::Side side); //Does a menu currently exist for the given army?
	static void Update(BehaviorArmy::Side side, Gamepad gamepad, float dt, Node *cursorMapPos); //Update the cursor selection for the given army based on gamepad input.
	static void Shutdown(); //Destroy all menus and all objects belonging to the menus.

private:
	static vector<PopupMenu*> menus; //Vector of currently existing menus.

	PopupMenu(BehaviorArmy *army, MenuType type); //Private constructor to prevent class from being instantiated outside of given parameters.
	void ConfigureMenu(); //Configure the menu to represent the correct team.
	static PopupMenu* GetMenu(BehaviorArmy::Side side); //Return the menu belonging to the given army.

	MenuType type; //The type of menu that this is.
	BehaviorArmy::Side side; //The army that this menu belongs to.
	BehaviorArmy* army; //The army that this menu belongs to.
	int selectedButton = 0; //Which button is selected on this menu.

	Node *buildingMapPos; //The position of this menu's building

	bool configured = false; //Is this menu configured yet?
};

//------------------------------------------------------------------------------
