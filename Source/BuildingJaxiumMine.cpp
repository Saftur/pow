#include "stdafx.h"
#include "BuildingJaxiumMine.h"
#include "GameObjectManager.h"
#include "LevelManager.h"

BuildingJaxiumMine::BuildingJaxiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, JaxiumMine, Basic, 10.0f, 100.0f, Jaxium, pos)
{
}

Component * BuildingJaxiumMine::Clone() const
{
	return new BuildingJaxiumMine(*this);
}

void BuildingJaxiumMine::BuildingUpdate(float dt){
	army->AddToFunds(fundsPerSecond * dt);
}
