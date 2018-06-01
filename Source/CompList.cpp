#include "stdafx.h"

#include "Health.h"
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

#include "CreateBuildingButton.h"
#include "ResearchButton.h"
#include "CreateUnitButton.h"
#include "TurretUpgradeButton.h"
#include "SellBuildingButton.h"


void CompList::List() {
	// Use for every new Behavior:
	LevelManager::AddComponentType("Cursor", new Cursor());
	LevelManager::AddComponentType("BehaviorArmy", new BehaviorArmy());
	LevelManager::AddComponentType("BehaviorUnit", new BehaviorUnit());
	LevelManager::AddComponentType("Map", new Map());
	LevelManager::AddComponentType("BehaviorTestBox", new BehaviorTestBox());
	LevelManager::AddComponentType("BehaviorProjectile", new BehaviorProjectile());
	
	//Building components.
	LevelManager::AddComponentType("CommandPost", new BuildingCommandPost(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("JaxiumMine", new BuildingJaxiumMine(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("NeoridiumMine", new BuildingNeoridiumMine(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("ResearchCenter", new BuildingResearchCenter(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("Teleporter", new BuildingTeleporter(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	LevelManager::AddComponentType("Turret", new BuildingTurret(BehaviorArmy::Side::sIllegal, { -1, -1 }));

	// Buttons
	LevelManager::AddComponentType("CreateBuildingButton", new CreateBuildingButton());
	LevelManager::AddComponentType("ResearchButton", new ResearchButton());
	LevelManager::AddComponentType("TurretUpgradeButton", new TurretUpgradeButton());
	LevelManager::AddComponentType("CreateUnitButton", new CreateUnitButton());
	LevelManager::AddComponentType("SellBuildingButton", new SellBuildingButton());

	LevelManager::AddComponentType("Health", new Health());
}
