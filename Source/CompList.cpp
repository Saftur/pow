#include "stdafx.h"

#include "CompList.h"
#include "LevelManager.h"
#include "Cursor.h"
#include "BehaviorArmy.h"
#include "BehaviorUnit.h"
#include "Map.h"
#include "BehaviorTestBox.h"
#include "BuildingCommandPost.h"
#include "BuildingJaxiumMine.h"
#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingTeleporter.h"
#include "BuildingTurret.h"


void CompList::List() {
	// Use for every new Behavior:
	//LevelManager::GetInstance().AddComponentType();
	LevelManager::AddComponentType("Cursor", new Cursor());
	LevelManager::AddComponentType("BehaviorArmy", new BehaviorArmy());
	LevelManager::AddComponentType("BehaviorUnit", new BehaviorUnit());
	LevelManager::AddComponentType("Map", new Map());
	LevelManager::AddComponentType("BehaviorTestBox", new BehaviorTestBox());

	//Building components.
	LevelManager::AddComponentType("CommandPost", new BuildingCommandPost(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("JaxiumMine", new BuildingJaxiumMine(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("NeoridiumMine", new BuildingNeoridiumMine(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("ResearchCenter", new BuildingResearchCenter(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("Teleporter", new BuildingTeleporter(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("Turret", new BuildingTurret(BehaviorArmy::Side::sIllegal, { -1, -1 }));
}