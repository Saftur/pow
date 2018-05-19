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
	enum BuildingType { JaxiumMine, NeoridiumMine, ResearchCenter, Spaceport, VehicleDepot, Turret, Teleporter, CommandPost, BuildingCount, Null = -1 };
	enum SpecialtyType { Basic, Advanced, Special, sCommandPost };
	enum CostType { Jaxium, Neoridium };

	Building(BehaviorArmy::Side side, BuildingType type, SpecialtyType specialtyType, float buildTime, float maxHealth, Vector2D pos, float jaxiumDropAmount, float neoridiumDropAmount);
	~Building();

	virtual Component* Clone() const = 0;

	static void InitializeBuildings(BehaviorArmy::Side side); //Initialize the buildings for the given army.

	void Update(float dt); //Update the generic building.
	virtual void BuildingUpdate(float dt) = 0; //Run the specific update on the building.

	virtual void OpenMenu(Vector2D cursorMapPos, Vector2D cursorScreenPos); //Open a menu for the building.

	float Variance(float value, float variance); //Return a float = the value +- a percentage of it based on the variance.

	static void Lock(BehaviorArmy::Side side, BuildingType type); //Lock the given building for the given army.
	static void Unlock(BehaviorArmy::Side side, BuildingType type); //Unlock the given building for the given army.
	static bool IsUnlocked(BehaviorArmy::Side side, BuildingType type); //Checks if the given building for the given army is unlocked.

	void SetHealth(float amount); //Set the building's health to some value.
	float GetHealth(); //Return the amount of health the building has.
	void Damage(float amount); //Lower the health of the building by X amount.
	void Heal(float amount); //Increase the health of the building by X amount.

	static map<BuildingType, float> buildingCost;
	static vector<GameObject*> allBuildings;

	BehaviorArmy *army; //Which army owns this building.
	BehaviorArmy::Side side; //Which side is this army on.

	BuildingType buildingType; //Building type.
	SpecialtyType specialtyType; //What kind of building is this.
	float buildTime; //How long it takes to create this building in seconds.

	AEGfxVertexList *mesh = nullptr; //Mesh for this building.
	AEGfxTexture *texture = nullptr; //Texture for this building.

	Vector2D mapPos; //The position of the building on the map.

	bool firstRun = true; //Allows each building to run things once when the building finishes being built.

private:
	static map<BehaviorArmy::Side, bool[BuildingCount]> buildings; //Map of boolean arrays each belonging to an army.

	float buildTimeRemaining; //How long until the building will be finnished creating.

	Vector2D originalScale; //The original scale of the building, used to play a temporary building "animation".

	float maxHealth; //The maximum health of the building.
	float health; //The health of the building.

	float jaxiumDropAmount; //The amount of Jaxium to drop on death.
	float neoridiumDropAmount; //The amount of Neoridium to drop on death.
};

//------------------------------------------------------------------------------
