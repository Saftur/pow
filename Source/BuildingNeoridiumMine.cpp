#include "stdafx.h"
#include "BuildingNeoridiumMine.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include <vector>
using std::vector;

BuildingNeoridiumMine::BuildingNeoridiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, JaxiumMine, Basic, 10.0f, 100.0f, Jaxium, pos)
{
}

Component * BuildingNeoridiumMine::Clone() const
{
	return new BuildingNeoridiumMine(*this);
}

void BuildingNeoridiumMine::BuildingUpdate(float dt){
	neoridium[side] += (fundsPerSecond * dt);
}

bool BuildingNeoridiumMine::TakeNeoridium(BehaviorArmy::Side side, float amount)
{
	if (neoridium[side] >= amount) {
		neoridium[side] -= amount;
		return true;
	}
	return false;
}

void BuildingNeoridiumMine::AddNeoridium(BehaviorArmy::Side side, float amount)
{
	neoridium[side] += amount;
}
