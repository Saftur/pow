#include "stdafx.h"
#include "AEEngine.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "BuildingJaxiumMine.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "GridManager.h"
#include <vector>
#include "Crystal.h"

using std::vector;

BuildingJaxiumMine::BuildingJaxiumMine(BehaviorArmy::Side side, Vector2D pos) : Building(side, JaxiumMine, Basic, 3.5f, 1000.0f, pos, 50, 0)
{
}

Component * BuildingJaxiumMine::Clone() const
{
	return new BuildingJaxiumMine(*this);
}

bool BuildingJaxiumMine::AddWorker()
{
	if (workers < 3) {
		workers++;
		return true;
	}
	return false;
}

bool BuildingJaxiumMine::RemoveWorker()
{
	if (workers > 0) {
		workers--;
		return true;
	}
	return false;
}

void BuildingJaxiumMine::BuildingUpdate(float dt){
	//If there is at least one unit working this mine.
	if (workers > 0) {
		//Decrement remaining spawn time for a crystal.
		remainingSpawnTime -= dt;

		//If a crystal is ready, spawn it.
		if (remainingSpawnTime <= 0) {

			//Get a list of all nearby nodes.
			vector<GridManager::Node*> nearbyNodes = GridManager::GetInstance().GetNeighbors(GridManager::GetInstance().GetNode((int)mapPos.x, (int)mapPos.y));

			//Find a random open node in the list of nearby nodes.
			unsigned nodeID = rand() % nearbyNodes.size();
			if (LevelManager::GetLayer(0)->GetObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
				if (obj->GetComponent<Crystal>()) {
					if (obj->GetComponent<Transform>()->GetTranslation() == GridManager::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID])) {
						return true;
					}
				}
				return false;
			}).size() > 0) return;

			remainingSpawnTime = Variance(crystalSpawnTime, crystalSpawnTimeVariance) / workers; //Reset the spawn timer.

			//Create a jaxium crystal on the node.
			GameObject *jaxium = new GameObject("Jaxium Crystal");
			Transform* transform = new Transform();
			transform->SetScale({ 25, 25 });
			transform->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(nearbyNodes[nodeID]));
			jaxium->AddComponent(transform);

			Crystal *jaxiumCrystal = new Crystal(Crystal::CrystalType::Jaxium, Variance(fundsPerCrystal, crystalWorthVariance));

			Sprite *sprite = new Sprite();
			jaxiumCrystal->texture = AEGfxTextureLoad("Data\\Assets\\Jaxium Crystal.png");
			SpriteSource* spriteSource = new SpriteSource(1, 1, jaxiumCrystal->texture);
			sprite->SetSpriteSource(spriteSource);
			jaxiumCrystal->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
			sprite->SetMesh(jaxiumCrystal->mesh);

			jaxium->AddComponent(sprite);
			jaxium->AddComponent(jaxiumCrystal);

			LevelManager::GetLayer(0)->GetObjectManager()->Add(*jaxium);
		}
	}
}
