#include "stdafx.h"

#include "CompList.h"
#include "LevelManager.h"
#include "Cursor.h"
#include "MenuCursor.h"
#include "BehaviorArmy.h"
#include "BehaviorUnit.h"
#include "Map.h"
#include "BehaviorTestBox.h"
#include "ArmyTraits.h"
#include "CustomPointCounter.h"
#include "CostDisplay.h"

#include "CustomizationMain.h"

#include "BuildingCommandPost.h"
#include "BuildingJaxiumMine.h"
#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingTeleporter.h"
#include "BuildingTurret.h"

#include "CreateBuildingButton.h"
#include "ResearchButton.h"
#include "CreateUnitButton.h"
#include "NewUnitButton.h"
#include "LoadArmyButton.h"
#include "SaveArmyButton.h"
#include "ChangeStatButton.h"
#include "ChangeWeaponAbilityButton.h"


#define NEWCOMP(name, comp) LevelManager::AddComponentType(name, comp)
void CompList::List() {
	NEWCOMP("Cursor", new Cursor());
	NEWCOMP("MenuCursor", new MenuCursor());
	NEWCOMP("BehaviorArmy", new BehaviorArmy());
	NEWCOMP("BehaviorUnit", new BehaviorUnit());
	NEWCOMP("Map", new Map());
	NEWCOMP("BehaviorTestBox", new BehaviorTestBox());
	NEWCOMP("BehaviorProjectile", new BehaviorProjectile());
	NEWCOMP("ArmyTraits", new ArmyTraits());
	NEWCOMP("CustomPointCounter", new CustomPointCounter());
	NEWCOMP("CostDisplay", new CostDisplay());

	//Level main components.
	NEWCOMP("CustomizationMain", new CustomizationMain());
	
	//Building components.
	NEWCOMP("CommandPost", new BuildingCommandPost(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	NEWCOMP("JaxiumMine", new BuildingJaxiumMine(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	NEWCOMP("NeoridiumMine", new BuildingNeoridiumMine(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	NEWCOMP("ResearchCenter", new BuildingResearchCenter(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	NEWCOMP("Teleporter", new BuildingTeleporter(BehaviorArmy::Side::sIllegal, { -1, -1 }));
	NEWCOMP("Turret", new BuildingTurret(BehaviorArmy::Side::sIllegal, { -1, -1 }));

	// Buttons
	NEWCOMP("CreateBuildingButton", new CreateBuildingButton());
	NEWCOMP("ResearchButton", new ResearchButton());
	NEWCOMP("CreateUnitButton", new CreateUnitButton());
	NEWCOMP("NewUnitButton", new NewUnitButton());
	NEWCOMP("LoadArmyButton", new LoadArmyButton());
	NEWCOMP("SaveArmyButton", new SaveArmyButton());
	NEWCOMP("ChangeStatButton", new ChangeStatButton());
	NEWCOMP("ChangeWeaponAbilityButton", new ChangeWeaponAbilityButton());
}
#undef NEWCOMP
