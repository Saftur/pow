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
#include "ArmyButton.h"
#include "Controls.h"

vector<PopupMenu*> PopupMenu::menus;

void PopupMenu::CreateMenu(BehaviorArmy *army, MenuType type, Node *buildingMapPos)
{
	BehaviorArmy::Side side = army->GetSide();
	//Destroy the menu for a given army if it exists, then create a new menu.
	if (Exists(side)) DestroyMenu(side);

	PopupMenu* menu = new PopupMenu(army, type, buildingMapPos);
	menus.push_back(menu);
}

void PopupMenu::DestroyMenu(BehaviorArmy::Side side)
{
	//Destroy the menu for a given army if it exists.
	for (unsigned i = 0; i < menus.size(); i++) {
		if (menus[i]->side == side) {
			Space::DeleteLayer(menus[i]->side);
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

void PopupMenu::Update(BehaviorArmy::Side side, Gamepad gamepad, float dt, Node *cursorMapPos)
{
	//Update which button is currently being selected on the menu belonging to the given army.
	PopupMenu* menu = GetMenu(side);
	if (menu && Space::GetLayer(menu->side)) {
		int buttons = Space::GetLayer(menu->side)->GetGameObjectManager()->GetObjectsByName("Button").size(); //Number of buttons in the menu.
		if (gamepad.GetButtonTriggered(BTN_MENU_LEFT) || AEInputCheckTriggered(VK_LEFT)) {
			if (menu->selectedButton <= 0) menu->selectedButton = buttons - 1;
			else menu->selectedButton--;
		}
		else if (gamepad.GetButtonTriggered(BTN_MENU_RIGHT)) {
			if (menu->selectedButton >= buttons - 1) menu->selectedButton = 0;
			else menu->selectedButton++;
		}

		//Update the cursor to the correct position and size;
		if (buttons > 0) {
			//If the sprite isn't on the cursor yet, the menu needs to be configured for the correct team.
			GameObject* cursor = Space::GetLayer(menu->side)->GetGameObjectManager()->GetObjectByName("Cursor");
			if (!menu->configured) menu->ConfigureMenu();

			//Find the button that we have selected and update the cursor.
			GameObject* selectedButton = Space::GetLayer(menu->side)->GetGameObjectManager()->GetObjectsByName("Button")[menu->selectedButton];
			Transform* selectedButtonTransform = selectedButton->GetComponent<Transform>();
			Transform* cursorTransform = cursor->GetComponent<Transform>();
			ArmyButton* button = selectedButton->GetChildComponent<ArmyButton>();
			cursorTransform->SetScale(selectedButtonTransform->GetScale() * 1.1f);
			cursorTransform->SetWorldTranslation(selectedButtonTransform->GetTranslation());

			//If we hit select, click the button and close the menu.	
			if (gamepad.GetButtonTriggered(BTN_MENU_SELECT) || AEInputCheckTriggered(VK_RETURN)) {
				button->SetMapPos(menu->buildingMapPos ? menu->buildingMapPos : cursorMapPos);
				button->Click(dt);
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
		Space::DeleteLayer(menus[i]->side);
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

PopupMenu::PopupMenu(BehaviorArmy *army, MenuType type, Node *buildingMapPos) : army(army), side(army->GetSide()), type(type), buildingMapPos(buildingMapPos)
{
	//Load the level for the correct type of level.
	map<string, void*> vars;
	vars["Army"] = army;
	vars["GridPosition"] = buildingMapPos;
	switch (type) {
	case Building:
		Space::LoadLayer(side, "BuildingMenu", vars, true);
		break;
	case Unit:
		break;
	case Research:
		Space::LoadLayer(side, "ResearchMenu", vars, true);
		break;
	case CommandPost:
		Space::LoadLayer(side, "CommandPostMenu", vars, true);
		break;
	case Turret:
		Space::LoadLayer(side, "TurretUpgradeMenu", vars, true);
		break;
	case SellBuilding:
		Space::LoadLayer(side, "SellBuilding", vars, true);
		break;
	}
	Space::GetLayer(side)->GetLevelManager()->Update(0);
	GameObject *camObj = Space::GetLayer(side)->GetGameObjectManager()->GetObjectByName("Camera");
	if (camObj) {
		if (side == BehaviorArmy::sRight)
			camObj->GetComponent<Camera>()->ChangePos({0.5f, 0.0f});
	}
}

void PopupMenu::ConfigureMenu() {
	configured = true;
	GameObject* cursor = Space::GetLayer(side)->GetGameObjectManager()->GetObjectByName("Cursor");

	if (side == BehaviorArmy::Side::sLeft) {
		cursor->GetComponent<Sprite>()->SetModulateColor({ 1, 0, 0, 1 });
	}
	else if(side == BehaviorArmy::Side::sRight) {
		cursor->GetComponent<Sprite>()->SetModulateColor({ 0, 0, 1, 1 });
	}

	Space::GetLayer(side)->GetGameObjectManager()->ForEachObject([&](GameObject* obj) {
		//Update the level to represent the correct team. (sLeft is Red team)
		if (side == BehaviorArmy::Side::sLeft) {
			Transform* t = obj->GetComponent<Transform>();
			t->SetTranslation(t->GetTranslation() + Vector2D( 0, 345 ));
		}
	});
}
