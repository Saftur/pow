//------------------------------------------------------------------------------
//
// File Name:	Building.h
// Author(s):	Brand Knutson
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include "Component.h"
#include "BehaviorArmy.h"

#include <map>
using std::map;

typedef BehaviorArmy BehaviorArmy;
typedef AEGfxVertexList AEGfxVertexList;
typedef AEGfxTexture AEGfxTexture;

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

class Building : public Component
{
public:
	enum BuildingType { JaxiumMine, NeoridiumMine, ResearchCenter, Spaceport, VehicleDepot, Turret, Teleporter, BuildingCount, Null = -1 };
	enum SpecialtyType { Basic, Advanced, Special };
	enum CostType { Jaxium, Neoridium };

	Building(BehaviorArmy::Side side, BuildingType type, SpecialtyType specialtyType, float buildTime, float buildCost, CostType costType, Vector2D pos);
	~Building();

	virtual Component* Clone() const = 0;

	static void InitializeBuildings(BehaviorArmy::Side side); //Initialize the buildings for the given army.

	static void Lock(BehaviorArmy::Side side, BuildingType type); //Lock the given building for the given army.
	static void Unlock(BehaviorArmy::Side side, BuildingType type); //Unlock the given building for the given army.
	static bool IsUnlocked(BehaviorArmy::Side side, BuildingType type); //Checks if the given building for the given army is unlocked.

	BehaviorArmy *army; //Which army owns this building.
	BehaviorArmy::Side side; //Which side is this army on.

	BuildingType buildingType; //Building type.
	SpecialtyType specialtyType; //What kind of building is this.
	float buildTime; //How long it takes to create this building in seconds.
	float buildCost; //How much does it take to create this building.

	AEGfxVertexList *mesh = nullptr; //Mesh for this building.
	AEGfxTexture *texture = nullptr; //Texture for this building.

	void Update(float dt); //Update the generic building.
	virtual void BuildingUpdate(float dt) = 0; //Run the specific update on the building.

private:
	static map<BehaviorArmy::Side, bool[BuildingCount]> buildings; //Map of boolean arrays each belonging to an army.

	float buildTimeRemaining; //How long until the building will be finnished creating.
};

//------------------------------------------------------------------------------
