//------------------------------------------------------------------------------
//
// File Name:	BuildingResearchCenter.h
// Author(s):	Brand Knutson
// Project:		Project 7
// Course:		CS230
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Building.h"
#include "BehaviorArmy.h"
#include "PopupMenu.h"
using std::map;


//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

class BuildingResearchCenter : public Building
{
public:
	enum Research { Spaceport = 3, VehicleDepot, Turret, Teleporter, ResearchCount = 4, Null = -1 };

	BuildingResearchCenter(BehaviorArmy::Side side, Vector2D pos);
	Component* Clone() const;

	void BuildingUpdate(float dt); //Update the building.

	void OpenMenu(); //Open a menu for the research center.

	static void Unlock(BehaviorArmy::Side side, Building::BuildingType type); //Research the building.

	static void InitializeResearchCost(); //Initialize the researchCost map.
	static float GetCost(Research search); //Get the cost of the research.
	static bool CanUnlock(BehaviorArmy::Side side, Research search); //Can this research be unlocked.

private:

	static map<BehaviorArmy::Side, map<Research, bool>> research; //Map of all research for each team and whether it is unlocked or not.
	static map<Research, float> researchCost;
};

//------------------------------------------------------------------------------
