#include "stdafx.h"

#include "CompList.h"
#include "LevelManager.h"
#include "Cursor.h"
#include "BehaviorArmy.h"
#include "BehaviorUnit.h"


void CompList::List() {
	// Use for every new Behavior:
	//LevelManager::GetInstance().AddComponentType();
	LevelManager::AddComponentType("Cursor", new Cursor());
	LevelManager::AddComponentType("BehaviorArmy", new BehaviorArmy());
	LevelManager::AddComponentType("BehaviorUnit", new BehaviorUnit());
}