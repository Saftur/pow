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
#include "ControlList.h"
#include <vector>

using std::vector;

//------------------------------------------------------------------------------

typedef class BehaviorArmy BehaviorArmy;
typedef class GameObject GameObject;
typedef class Sprite Sprite;

enum MenuType {Building, Unit};

// An example of the class to be defined in BehaviorAsteroid.h
class PopupMenu
{
public:
	static void CreateMenu(BehaviorArmy* army, MenuType type, Sprite* cursor); //Create a menu for the given army.
	static void DestroyMenu(BehaviorArmy* army); //Destroy a menu for the given army.
	static bool Exists(BehaviorArmy* army); //Does a menu currently exist for the given army?
	static void Update(BehaviorArmy* army, Gamepad gamepad, ControlList controlList, float dt); //Update the cursor selection for the given army based on gamepad input.
	static void Shutdown(); //Destroy all menus and all objects belonging to the menus.

private:
	static vector<PopupMenu*> menus; //Vector of currently existing menus.

	PopupMenu(BehaviorArmy* army, MenuType type, Sprite* cursor); //Private constructor to prevent class from being instantiated outside of given parameters.
	static void ConfigureMenu(BehaviorArmy* army, PopupMenu* menu = nullptr); //Configure the menu to represent the correct team.
	static PopupMenu* GetMenu(BehaviorArmy* army); //Return the menu belonging to the given army.

	MenuType type; //The type of menu that this is.
	BehaviorArmy* army; //The army that this menu belongs to.
	int menuLevelLayer = -1; //The layer that the menu has been loaded onto.
	int selectedButton = 0; //Which button is selected on this menu.

	Sprite* cursorSprite; //The sprite for the cursor for the team that created this menu.
};

//------------------------------------------------------------------------------
