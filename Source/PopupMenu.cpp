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

vector<PopupMenu*> PopupMenu::menus;

void PopupMenu::CreateMenu(BehaviorArmy * army, MenuType type, Sprite* cursor)
{
	//Destroy the menu for a given army if it exists, then create a new menu.
	if (Exists(army)) DestroyMenu(army);

	PopupMenu* menu = new PopupMenu(army, type, cursor);
	menus.push_back(menu);
}

void PopupMenu::DestroyMenu(BehaviorArmy * army)
{
	//Destroy the menu for a given army if it exists.
	for (unsigned i = 0; i < menus.size(); i++) {
		if (menus[i]->army == army) {
			LevelManager::UnloadLayer(menus[i]->menuLevelLayer);
			delete menus[i]->cursorSprite;
			delete menus[i];
			menus.erase(menus.begin() + i);
		}
	}
}

bool PopupMenu::Exists(BehaviorArmy * army)
{
	//Return true if a menu exists for the given army, else false.
	for (unsigned i = 0; i < menus.size(); i++) {
		if (menus[i]->army == army) return true;
	}
	return false;
}

void PopupMenu::Update(BehaviorArmy * army, Gamepad gamepad, ControlList controlList, float dt)
{
	//Update which button is currently being selected on the menu belonging to the given army.
	PopupMenu* menu = GetMenu(army);
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
			if (!cursor->GetComponent("Sprite")) ConfigureMenu(army);

			//Find the button that we have selected and update the cursor.
			GameObject* selectedButton = LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectsByName("Button")[menu->selectedButton];
			Transform* selectedButtonTransform = (Transform*)selectedButton->GetComponent("Transform");
			Transform* cursorTransform = (Transform*)cursor->GetComponent("Transform");
			cursorTransform->SetScale(selectedButtonTransform->GetScale() * 1.1f);
			cursorTransform->SetScreenTranslation(selectedButtonTransform->GetTranslation());

			//If we hit select, click the button and close the menu.	
			if (gamepad.GetButtonTriggered(controlList["Select.Manual.Select"]) || AEInputCheckTriggered(VK_RETURN)) {
				Button::ForceClick(*(Button*)selectedButton->GetComponent("Button"), dt);
				PopupMenu::DestroyMenu(army);
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
		delete menus[i]->cursorSprite;
		delete menus[i];
		menus.erase(menus.begin() + i);
	}
}

PopupMenu* PopupMenu::GetMenu(BehaviorArmy* army) {
	for (unsigned i = 0; i < menus.size(); i++) {
		if (menus[i]->army == army) return menus[i];
	}
	return nullptr;
}

bool Test(...) {
	return true;
}

PopupMenu::PopupMenu(BehaviorArmy * army, MenuType type, Sprite* cursor) : army(army), type(type)
{
	//Load the level for the correct type of level.
	switch (type) {
	case Building:
		LevelManager::LoadLayer(LevelManager::GetLayerCount(), "TestPopup", true);
		break;
	case Unit:
		break;
	}
	menuLevelLayer = LevelManager::GetLayerCount() - 1;

	//Put a sprite on our cursor that matches the sprite on the cursor for the army that is creating this menu.
	cursorSprite = (Sprite*)cursor->Clone();
}

void PopupMenu::ConfigureMenu(BehaviorArmy* army, PopupMenu* menu) {
	if (!menu) {
		menu = GetMenu(army);
	}
	if (menu) {
		GameObject* cursor = LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectByName("Cursor");
		cursor->AddComponent(menu->cursorSprite->Clone());
		((Sprite*)cursor->GetComponent("Sprite"))->SetAlpha(1.0f);

		//Update the level to represent the correct team. (Cursor1 is Red team)
		if (!strcmp(menu->cursorSprite->GetParent()->GetName(), "Cursor1")) {
			int count = (LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectsWithFilter(Test)).size();
			for (int i = 0; i < count; i++) {
				Transform* t = (Transform*)((LevelManager::GetLayer(menu->menuLevelLayer)->GetObjectManager()->GetObjectsWithFilter(Test)[i])->GetComponent("Transform"));
				t->SetTranslation(t->GetTranslation().operator+({ 0, 345 }));
			}
		}
		else {

		}
	}
}
