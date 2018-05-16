#include "stdafx.h"
#include "BuildingNeoridiumMine.h"
#include "Space.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "GameObject.h"
#include "Crystal.h"
#include "AEEngine.h"
#include "Mesh.h"
#include "SpriteSource.h"

std::map<BehaviorArmy::Side, float> BuildingNeoridiumMine::neoridium;

BuildingNeoridiumMine::BuildingNeoridiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, NeoridiumMine, Advanced, 3.5f, 1000.0f, pos, 0, 50)
{
}

Component * BuildingNeoridiumMine::Clone() const
{
	return new BuildingNeoridiumMine(*this);
}

void BuildingNeoridiumMine::BuildingUpdate(float dt){
	//If there is at least one unit working this mine.
	if (workers > 0) {
		//Decrement remaining spawn time for a crystal.
		remainingSpawnTime -= dt;

		//If a crystal is ready, spawn it.
		if (remainingSpawnTime <= 0) {

			//Get a list of all nearby nodes.
			vector<Grid::Node> nearbyNodes = Grid::GetInstance().GetNeighbors(Grid::GetInstance().GetNode((int)mapPos.x, (int)mapPos.y));

			//Find a random open node in the list of nearby nodes.
			unsigned nodeID = rand() % nearbyNodes.size();
			if (Space::GetLayer(0)->GetGameObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
				if (obj->GetComponent<Crystal>()) {
					if (obj->GetComponent<Transform>()->GetTranslation() == Grid::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID])) {
						return true;
					}
				}
				return false;
			}).size() > 0) return;

			remainingSpawnTime = Variance(crystalSpawnTime, crystalSpawnTimeVariance) / workers; //Reset the spawn timer.

			//Create a neoridium crystal on the node.
			GameObject *neoridium = new GameObject("Neoridium Crystal");
			Transform* transform = new Transform();
			transform->SetScale({ 25, 25 });
			transform->SetTranslation(Grid::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID]));
			neoridium->AddComponent(transform);

			Crystal *neoridiumCrystal = new Crystal(Crystal::CrystalType::Neoridium, Variance(fundsPerCrystal, crystalWorthVariance));

			Sprite *sprite = new Sprite();
			neoridiumCrystal->texture = AEGfxTextureLoad("Data\\Assets\\Neoridium Crystal.png");
			SpriteSource* spriteSource = new SpriteSource(1, 1, neoridiumCrystal->texture);
			sprite->SetSpriteSource(spriteSource);
			neoridiumCrystal->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
			sprite->SetMesh(neoridiumCrystal->mesh);

			neoridium->AddComponent(sprite);
			neoridium->AddComponent(neoridiumCrystal);

			Space::GetLayer(0)->GetGameObjectManager()->Add(*neoridium);
		}
	}
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

bool BuildingNeoridiumMine::AddWorker()
{
	if (workers < 3) {
		workers++;
		return true;
	}
	return false;
}

bool BuildingNeoridiumMine::RemoveWorker()
{
	if (workers > 0) {
		workers--;
		return true;
	}
	return false;
}
