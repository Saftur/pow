#include "stdafx.h"

#include "CompList.h"
#include "LevelManager.h"
#include "Cursor.h"
#include "BehaviorArmy.h"
#include "BehaviorUnit.h"
#include "Map.h"
#include "BehaviorTestBox.h"
#include "BuildingCommandPost.h"

#include "CreateBuildingButton.h"
#include "ResearchButton.h"
#include "CreateUnitButton.h"


void CompList::List() {
	// Use for every new Behavior:
	LevelManager::AddComponentType("Cursor", new Cursor());
	LevelManager::AddComponentType("BehaviorArmy", new BehaviorArmy());
	LevelManager::AddComponentType("BehaviorUnit", new BehaviorUnit());
	LevelManager::AddComponentType("Map", new Map());
	LevelManager::AddComponentType("BehaviorTestBox", new BehaviorTestBox());

	// Buttons
	LevelManager::AddComponentType("CreateBuildingButton", new CreateBuildingButton());
	LevelManager::AddComponentType("ResearchButton", new ResearchButton());
	LevelManager::AddComponentType("CreateUnitButton", new CreateUnitButton());
}