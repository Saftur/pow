#include "stdafx.h"

#include "CompList.h"
#include "LevelManager.h"
#include "Cursor.h"
#include "BehaviorArmy.h"
#include "BehaviorUnit.h"
#include "Map.h"
#include "BehaviorTestBox.h"
#include "BuildingCommandPost.h"


void CompList::List() {
	// Use for every new Behavior:
	//LevelManager::GetInstance().AddComponentType();
	LevelManager::AddComponentType("Cursor", new Cursor());
	LevelManager::AddComponentType("BehaviorArmy", new BehaviorArmy());
	LevelManager::AddComponentType("BehaviorUnit", new BehaviorUnit());
	LevelManager::AddComponentType("Map", new Map());
	LevelManager::AddComponentType("BehaviorTestBox", new BehaviorTestBox());
	LevelManager::AddComponentType("BehaviorProjectile", new BehaviorProjectile());
}