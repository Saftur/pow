#include "stdafx.h"
#include "BuildingResearchCenter.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "BuildingNeoridiumMine.h"

map<BehaviorArmy::Side, map<BuildingResearchCenter::Research, bool>> BuildingResearchCenter::research;

map<BuildingResearchCenter::Research, float> BuildingResearchCenter::researchCost;

BuildingResearchCenter::BuildingResearchCenter(BehaviorArmy::Side side, Vector2D pos) : Building(side, ResearchCenter, Basic, 3.5f, 1000.0f, pos, 0, 50)
{
	if (!IsUnlocked(side, NeoridiumMine)) Unlock(side, NeoridiumMine);
}

Component * BuildingResearchCenter::Clone() const
{
	return new BuildingResearchCenter(*this);
}

void BuildingResearchCenter::BuildingUpdate(float dt)
{
}

void BuildingResearchCenter::OpenMenu(Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
	PopupMenu::CreateMenu(side, PopupMenu::MenuType::Research, cursorMapPos, cursorScreenPos);
}

void BuildingResearchCenter::Unlock(BehaviorArmy::Side side, Building::BuildingType type)
{
	if (BuildingNeoridiumMine::TakeNeoridium(side, GetCost((Research)type))) Building::Unlock(side, type);
}

void BuildingResearchCenter::InitializeResearchCost()
{
	researchCost[Spaceport] = 200.0f;
	researchCost[VehicleDepot] = 200.0f;
	researchCost[Turret] = 200.0f;
	researchCost[Teleporter] = 200.0f;
}

float BuildingResearchCenter::GetCost(Research search)
{
	return researchCost[search];
}
