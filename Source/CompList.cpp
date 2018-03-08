#include "stdafx.h"

#include "CompList.h"
#include "LevelManager.h"
#include "BehaviorArmy.h"
#include "BehaviorUnit.h"
#include "Map.h"


void CompList::List() {
	// Use for every new Behavior:
	//LevelManager::GetInstance().AddComponentType();
	LevelManager::AddComponentType("BehaviorArmy", new BehaviorArmy());
	LevelManager::AddComponentType("BehaviorUnit", new BehaviorUnit());
	LevelManager::AddComponentType("Map", new Map());
}