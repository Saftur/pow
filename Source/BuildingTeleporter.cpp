#include "stdafx.h"
#include "AEEngine.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "BuildingTeleporter.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "GridManager.h"
#include <vector>
#include "Pathfinder.h"

using std::vector;

map<BehaviorArmy::Side, unsigned> BuildingTeleporter::teleporters;

BuildingTeleporter::BuildingTeleporter(BehaviorArmy::Side side, Vector2D pos) : Building(side, Teleporter, Special, 0.0f, 250.0f, pos, 0, 50)
{
	if (teleporters[side] >= 2) throw(0);
	teleporters[side]++;
}

BuildingTeleporter::~BuildingTeleporter()
{
	if (exit) {
		exit->Destroy();
		exit->GetComponent<BuildingTeleporter>()->exit = nullptr;
		exit = nullptr;
	}
	teleporters[side]--;
}

Component * BuildingTeleporter::Clone() const
{
	return new BuildingTeleporter(*this);
}

void BuildingTeleporter::BuildingUpdate(float dt){
	if (firstRun) {
		firstRun = !SetExit();
	}
	else if (exit) {
		if (teleportationsAvailable <= 0) {
			GetParent()->Destroy();
			return;
		}

		///TODO: Reset lastTeleportedObject if the object on this teleporter is not the last teleported object.
		GameObject* unit = GridManager::GetInstance().GetOccupant(mapPos);
		if (lastTeleportedObject != unit) lastTeleportedObject = nullptr;
		 
		//Check if the exit is currently open.
		Vector2D exitGridPos = exit->GetComponent<BuildingTeleporter>()->mapPos;
		if (!GridManager::GetInstance().GetOccupant(exitGridPos)) {

			//Make sure that we arent teleporting the unit back and forth.
			if (lastTeleportedObject != unit) {
				///TODO: If unit is currently running pathfinding to move somewhere, do not teleport
				teleportationsAvailable--; //Lower the number of teleportations available.
				exit->GetComponent<BuildingTeleporter>()->lastTeleportedObject = unit;
				unit->GetComponent<Transform>()->SetTranslation(exit->GetComponent<Transform>()->GetTranslation());
				
				BehaviorUnit* unitBehavior = unit->GetComponent<BehaviorUnit>();
				unitBehavior->SetGridPos(exitGridPos);

				//Find an open node by the exit.
				vector<GridManager::Node*> nodesByExit = GridManager::GetInstance().GetNeighbors(GridManager::GetInstance().GetNode(exitGridPos));
				GridManager::Node* pathNode = nullptr;
				for (GridManager::Node *node : nodesByExit) {
					if (!GridManager::GetInstance().GetOccupant(node))
					{
						pathNode = node;
						break;
					}
				}
				///TODO: Give the unit a path to the chosen node by the exit.
				//unitBehavior->StopPathfinding();
				if(pathNode) unitBehavior->SetPath(Pathfinder::FindPath(unitBehavior->GetNode(), pathNode));
			}
		}
	}
}

bool BuildingTeleporter::SetExit()
{
	return (LevelManager::GetLayer(0)->GetObjectManager()->GetObjectsWithFilter([&](GameObject *obj) {
		BuildingTeleporter *teleporter = obj->GetComponent<BuildingTeleporter>();
		if (teleporter && teleporter != this && teleporter->side == side) {
			exit = obj;
			return true;
		}
		return false;
	})).size();
}