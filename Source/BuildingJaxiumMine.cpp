#include "stdafx.h"
#include "BuildingJaxiumMine.h"
#include "GameObjectManager.h"
#include "LevelManager.h"

BuildingJaxiumMine::BuildingJaxiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, JaxiumMine, Basic, 3.5f, 1000.0f, pos, 50, 0)
{
}

Component * BuildingJaxiumMine::Clone() const
{
	return new BuildingJaxiumMine(*this);
}

void BuildingJaxiumMine::BuildingUpdate(float dt){
	///TODO: Drop some money instead of simply adding it to the balance.
	army->AddToFunds(fundsPerSecond * dt);
}
