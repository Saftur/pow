#include "stdafx.h"
#include "BuildingJaxiumMine.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include <vector>
using std::vector;

BuildingJaxiumMine::BuildingJaxiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, JaxiumMine, Advanced, 15.0f, 200.0f, Jaxium, pos)
{
}

Component * BuildingJaxiumMine::Clone() const
{
	return new BuildingJaxiumMine(*this);
}

void BuildingJaxiumMine::BuildingUpdate(float dt){
	army->AddToFunds(fundsPerSecond * dt);
}
