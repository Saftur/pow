#include "stdafx.h"
#include "BuildingNeoridiumMine.h"
#include "GameObjectManager.h"
#include "LevelManager.h"

std::map<BehaviorArmy::Side, float> BuildingNeoridiumMine::neoridium;

BuildingNeoridiumMine::BuildingNeoridiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, NeoridiumMine, Advanced, 3.5f, 1000.0f, pos, 0, 50)
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
