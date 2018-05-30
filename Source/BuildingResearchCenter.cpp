//------------------------------------------------------------------------------
//
// File Name:	BuildingResearchCenter.cpp
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BuildingResearchCenter.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "BuildingNeoridiumMine.h"

map<BehaviorArmy::Side, map<BuildingResearchCenter::Research, bool>> BuildingResearchCenter::research;

map<BuildingResearchCenter::Research, float> BuildingResearchCenter::researchCost;

BuildingResearchCenter::BuildingResearchCenter(BehaviorArmy::Side side, Vector2D pos) : Building(side, ResearchCenter, Basic, 3.5f, pos, 0, 50)
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

void BuildingResearchCenter::OpenMenu()
{
	PopupMenu::CreateMenu(army, PopupMenu::MenuType::Research, GridManager::GetInstance().GetNode(mapPos));
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
