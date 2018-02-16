#include "stdafx.h"

#include "CompList.h"
#include "LevelManager.h"
#include "BehaviorArmy.h"
#include "BehaviorUnit.h"


void CompList::List() {
	// Use for every new Behavior:
	//LevelManager::GetInstance().AddComponentType();
	LevelManager::GetInstance().AddComponentType("BehaviorArmy", new BehaviorArmy());
	LevelManager::GetInstance().AddComponentType("BehaviorUnit", new BehaviorUnit());
}