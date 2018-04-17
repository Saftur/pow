//------------------------------------------------------------------------------
//
// File Name:	BuildingMenu.cpp
// Author(s):	Brand Knutson
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
#include "GameObject.h" 
#include "AEEngine.h" 
#include "GameObjectManager.h" 
#include "LevelManager.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Mesh.h"
#include "Engine.h"
#include "PopupMenu.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Button.h"
#include "BehaviorArmy.h"
#include "Building.h"

vector<PopupMenu*> PopupMenu::menus;

void PopupMenu::CreateMenu(BehaviorArmy::Side side, MenuType type, Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
	//Destroy the menu for a given army if it exists, then create a new menu.
	if (Exists(side)) DestroyMenu(side);

	PopupMenu* menu = new PopupMenu(side, type);
	menu->armyCursorMapPos = cursorMapPos;
	menu->armyCursorScreenPos = cursorScreenPos;
	menus.push_back(menu);
}

void PopupMenu::DestroyMenu(BehaviorArmy::Side side)
{
	//Destroy the menu for a given army if it exists.
	for (unsigned i = 0; i < menus.size(); i++) {
		if (menus[i]->side == side) {
			LevelManager::UnloadLayer(menus[i]->menuLevelLayer);
			delete menus[i];
			menus.erase(menus.begin() + i);
		}
	}
}

bool PopupMenu::Exists(BehaviorArmy::Side side)
{
	//Return true if a menu exists for the given army, else false.
	for (unsigned i = 0; i < menus.size(); i++) {
		if (menus[i]->side == side) return true;
	}
	return false;
}

void PopupMenu::Update(BehaviorArmy::Side side, Gamepad gamepad, ControlList controlList, float dt)
{
	//Update which button is currently being selected on the menu belonging to the given army.
	PopupMenu* menu = GetMenu(side);
	if (menu && LevelManager::GetLayer(menu->menuLevelLayer)) {
		int buttons = LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectsByName("Button").size(); //Number of buttons in the menu.
		if (gamepad.GetButtonTriggered(controlList["CamLeft"]) || AEInputCheckTriggered(VK_LEFT)) {
			if (menu->selectedButton <= 0) menu->selectedButton = buttons - 1;
			else menu->selectedButton--;
		}
		else if (gamepad.GetButtonTriggered(controlList["CamRight"])) {
			if (menu->selectedButton >= buttons - 1) menu->selectedButton = 0;
			else menu->selectedButton++;
		}

		//Update the cursor to the correct position and size;
		if (buttons > 0) {
			//If the sprite isn't on the cursor yet, the menu needs to be configured for the correct team.
			GameObject* cursor = LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectByName("Cursor");
			if (!GetMenu(side)->configured) ConfigureMenu(side);

			//Find the button that we have selected and update the cursor.
			GameObject* selectedButton = LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectsByName("Button")[menu->selectedButton];
			Transform* selectedButtonTransform = (Transform*)selectedButton->GetComponent("Transform");
			Transform* cursorTransform = (Transform*)cursor->GetComponent("Transform");
			cursorTransform->SetScale(selectedButtonTransform->GetScale() * 1.1f);
			cursorTransform->SetScreenTranslation(selectedButtonTransform->GetTranslation());

			//If we hit select, click the button and close the menu.	
			if (gamepad.GetButtonTriggered(controlList["Select.Manual.Select"]) || AEInputCheckTriggered(VK_RETURN)) {
				Button::ForceClick(*(Button*)selectedButton->GetComponent("Button"), dt, 3, side, menu->armyCursorMapPos, menu->armyCursorScreenPos);
				PopupMenu::DestroyMenu(side);
				return;
			}
		}
	}
}

void PopupMenu::Shutdown()
{
	//Destroy the menus and everything on them.
	for (unsigned i = 0; i < menus.size(); i++) {
		LevelManager::UnloadLayer(menus[i]->menuLevelLayer);
		delete menus[i];
		menus.erase(menus.begin() + i);
	}
}

PopupMenu* PopupMenu::GetMenu(BehaviorArmy::Side side) {
	for (unsigned i = 0; i < menus.size(); i++) {
		if (menus[i]->side == side) return menus[i];
	}
	return nullptr;
}

PopupMenu::PopupMenu(BehaviorArmy::Side side, MenuType type) : side(side), type(type)
{
	//Load the level for the correct type of level.
	switch (type) {
	case Building:
		LevelManager::LoadLayer(LevelManager::GetLayerCount(), "BuildingMenu", true);
		break;
	case Unit:
		break;
	case Research:
		break;
	}
	menuLevelLayer = LevelManager::GetLayerCount() - 1;
}

void PopupMenu::ConfigureMenu(BehaviorArmy::Side side, PopupMenu* menu) {
	if (!menu) {
		menu = GetMenu(side);
	}
	if (menu) {
		menu->configured = true;
		GameObject* cursor = LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectByName("Cursor");

		(LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
			//Update the level to represent the correct team. (sLeft is Red team)
			if (side == BehaviorArmy::Side::sLeft) {
				((Sprite*)cursor->GetComponent("Sprite"))->SetModulateColor({ 1, 0, 0, 1 });

				Transform* t = (Transform*)obj->GetComponent("Transform");
				t->SetTranslation(t->GetTranslation().operator+({ 0, 345 }));
			}
			else if(side == BehaviorArmy::Side::sRight) {
				((Sprite*)cursor->GetComponent("Sprite"))->SetModulateColor({ 0, 0, 1, 1 });
			}

			Button* button = (Button*)obj->GetComponent("Button");
			if (button) {
				//If this button creates a building, check if the building it creates is locked and if it has been researched.
				Building::BuildingType buildingType = Button::buildingType[button->effectName];
				if (buildingType != Building::BuildingType::Null && !Building::IsUnlocked(side, buildingType)) button->active = false;

				//If this button researches something, check if we can unlock it.
				BuildingResearchCenter::Research researchType = Button::researchType[button->effectName];
				if (researchType != BuildingResearchCenter::Research::Null) {
					vector<GameObject*> disableButtons = LevelManager::GetLayer(0)->GetObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
						BehaviorArmy* army = (BehaviorArmy*)obj->GetComponent("BehaviorArmy");
						//If we found the army we want check if the building is already unlocked or if we can't afford to unlock it.
						if (army && army->GetSide() == side) {
							if (Building::IsUnlocked(side, buildingType) || army->GetFunds < BuildingResearchCenter::GetCost(researchType)) button->active = false;
						}
						return false;
					});
				}
			}
			return false;
		}));
	}
}
