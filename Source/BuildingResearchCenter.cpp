#include "stdafx.h"
#include "BuildingResearchCenter.h"
#include "GameObjectManager.h"
#include "LevelManager.h"

map<BehaviorArmy::Side, map<BuildingResearchCenter::Research, bool>> BuildingResearchCenter::research;

map<BuildingResearchCenter::Research, float> BuildingResearchCenter::researchCost;

BuildingResearchCenter::BuildingResearchCenter(BehaviorArmy::Side side, Vector2D pos) : Building(side, ResearchCenter, Basic, 20.0f, 250.0f, Jaxium, pos)
{
}

Component * BuildingResearchCenter::Clone() const
{
	return new BuildingResearchCenter(*this);
}

void BuildingResearchCenter::BuildingUpdate(float dt)
{
}

void BuildingResearchCenter::OpenResearchMenu(PopupMenu::MenuType type, Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
	PopupMenu::CreateMenu(side, type, cursorMapPos, cursorScreenPos);
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
