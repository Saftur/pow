//------------------------------------------------------------------------------
//
// File Name:	BuildingMenu.cpp
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h" 
#include "AEEngine.h" 
#include "Space.h"
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
#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"
#include "Controls.h"

vector<PopupMenu*> PopupMenu::menus;

void PopupMenu::CreateMenu(BehaviorArmy::Side side, MenuType type)
{
	//Destroy the menu for a given army if it exists, then create a new menu.
	if (Exists(side)) DestroyMenu(side);

	PopupMenu* menu = new PopupMenu(side, type);
	menus.push_back(menu);
}

void PopupMenu::CreateMenu(BehaviorArmy::Side side, MenuType type, Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
	//Destroy the menu for a given army if it exists, then create a new menu.
	if (Exists(side)) DestroyMenu(side);

	PopupMenu* menu = new PopupMenu(side, type);
	menu->overrideCursorPosition = true;
	menu->armyCursorMapPos = cursorMapPos;
	menu->armyCursorScreenPos = cursorScreenPos;
	menus.push_back(menu);
}

void PopupMenu::DestroyMenu(BehaviorArmy::Side side)
{
	//Destroy the menu for a given army if it exists.
	for (unsigned i = 0; i < menus.size(); i++) {
		if (menus[i]->side == side) {
			Space::DeleteLayer(menus[i]->menuLevelLayer);
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

void PopupMenu::Update(BehaviorArmy::Side side, Gamepad gamepad, float dt, Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
	//Update which button is currently being selected on the menu belonging to the given army.
	PopupMenu* menu = GetMenu(side);
	if (menu && Space::GetLayer(menu->menuLevelLayer)) {
		int buttons = Space::GetLayer(menu->menuLevelLayer)->GetGameObjectManager()->GetObjectsByName("Button").size(); //Number of buttons in the menu.
		if (gamepad.GetButtonTriggered(MENU_LEFT) || AEInputCheckTriggered(VK_LEFT)) {
			if (menu->selectedButton <= 0) menu->selectedButton = buttons - 1;
			else menu->selectedButton--;
		}
		else if (gamepad.GetButtonTriggered(MENU_RIGHT)) {
			if (menu->selectedButton >= buttons - 1) menu->selectedButton = 0;
			else menu->selectedButton++;
		}

		//Update the cursor to the correct position and size;
		if (buttons > 0) {
			//If the sprite isn't on the cursor yet, the menu needs to be configured for the correct team.
			GameObject* cursor = Space::GetLayer(menu->menuLevelLayer)->GetGameObjectManager()->GetObjectByName("Cursor");
			if (!GetMenu(side)->configured) ConfigureMenu(side);

			//Find the button that we have selected and update the cursor.
			GameObject* selectedButton = Space::GetLayer(menu->menuLevelLayer)->GetGameObjectManager()->GetObjectsByName("Button")[menu->selectedButton];
			Transform* selectedButtonTransform = (Transform*)selectedButton->GetComponent("Transform");
			Transform* cursorTransform = (Transform*)cursor->GetComponent("Transform");
			cursorTransform->SetScale(selectedButtonTransform->GetScale() * 1.1f);
			cursorTransform->SetWorldTranslation(selectedButtonTransform->GetTranslation());

			//If we hit select, click the button and close the menu.	
			if (gamepad.GetButtonTriggered(MENU_SELECT) || AEInputCheckTriggered(VK_RETURN)) {
				if(menu->overrideCursorPosition) Button::ForceClick(*(Button*)selectedButton->GetComponent<Button>(), dt, 4, side, 
					menu->armyCursorMapPos, menu->armyCursorScreenPos, menu->army);
				else Button::ForceClick(*(Button*)selectedButton->GetComponent<Button>(), dt, 4, side, cursorMapPos, cursorScreenPos, menu->army);
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
		Space::DeleteLayer(menus[i]->menuLevelLayer);
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
		Space::LoadLayer(Space::GetLayerCount(), "BuildingMenu", true);
		break;
	case Unit:
		break;
	case Research:
		Space::LoadLayer(Space::GetLayerCount(), "ResearchMenu", true);
		break;
	case CommandPost:
		Space::LoadLayer(Space::GetLayerCount(), "CommandPostMenu", true);
		break;
	case Turret:
		Space::LoadLayer(Space::GetLayerCount(), "TurretUpgradeMenu", true);
		break;
	}
	Space::GetLayer(Space::GetLayerCount()-1)->GetLevelManager()->Update(0);
	GameObject *camObj = Space::GetLayer(Space::GetLayerCount()-1)->GetGameObjectManager()->GetObjectByName("Camera");
	if (camObj) {
		if (side == BehaviorArmy::sRight)
			camObj->GetComponent<Camera>()->ChangePos({0.5f, 0.0f});
	}
	menuLevelLayer = Space::GetLayerCount() - 1;
}

void PopupMenu::ConfigureMenu(BehaviorArmy::Side side, PopupMenu* menu) {
	if (!menu) {
		menu = GetMenu(side);
	}
	if (menu) {
		menu->configured = true;
		GameObject* cursor = Space::GetLayer(menu->menuLevelLayer)->GetGameObjectManager()->GetObjectByName("Cursor");

		vector<GameObject*> findArmy = Space::GetLayer(0)->GetGameObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
			BehaviorArmy* army = obj->GetComponent<BehaviorArmy>();
			//If we found the army we want, set the menu's army equal to it.
			if (army && army->GetSide() == side) {
				menu->army = army;
			}
			return false;
		});

		(Space::GetLayer(menu->menuLevelLayer)->GetGameObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
			//Update the level to represent the correct team. (sLeft is Red team)
			if (side == BehaviorArmy::Side::sLeft) ((Sprite*)cursor->GetComponent("Sprite"))->SetModulateColor({ 1, 0, 0, 1 });
			else if (side == BehaviorArmy::Side::sRight) ((Sprite*)cursor->GetComponent("Sprite"))->SetModulateColor({ 0, 0, 1, 1 });

			Button* button = (Button*)obj->GetComponent("Button");
			if (button) {
				//Check if the building is already unlocked or if we can't afford to unlock it.
				Building::BuildingType buildingType = Button::buildingType[button->effectName];
				BuildingResearchCenter::Research researchType = Button::researchType[button->effectName];
				//If this button creates a building, check if the building it creates is locked and if it has been researched, and if we can afford it.
				if (researchType == BuildingResearchCenter::Research::Null && buildingType != Building::BuildingType::Null
					&& (!Building::IsUnlocked(side, buildingType) || menu->army->GetFunds() < Building::buildingCost[buildingType])) button->active = false;

				//If this button researches something, check if we can unlock it.
				if (researchType != BuildingResearchCenter::Research::Null) {
					if (Building::IsUnlocked(side, buildingType) || BuildingNeoridiumMine::neoridium[side] < BuildingResearchCenter::GetCost(researchType)) button->active = false;
				}
			}
			return false;
		}));
	}
}
