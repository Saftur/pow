#include "stdafx.h"

#include "ButtonEffects.h"

#include <vector>
#include <algorithm>
#include <AEEngine.h>
#include "Button.h"
#include "Trace.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "SpriteSource.h"
#include "Space.h"
#include "Mesh.h"
#include "GridManager.h"

//Building includes.
#include "BuildingNeoridiumMine.h"
#include "BuildingJaxiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingTeleporter.h"

using std::vector;

void ButtonEffects::List()
{
	/*
	// Add game-side button effects here using:
	// Button::AddClickEffect([name], [effect function]);
	Button::AddClickEffect("KillAllUnits", KillAllUnits);

	//Create Building Effects
	Button::AddClickEffect("CreateJaxiumMine", CreateJaxiumMineEffect);
	Button::AddClickEffect("CreateNeoridiumMine", CreateNeoridiumMineEffect);
	Button::AddClickEffect("CreateResearchCenter", CreateResearchCenterEffect);
	Button::AddClickEffect("CreateTeleporter", CreateTeleporterEffect);
	///TODO: Implement Click Effects for the remaining buildings.

	//Create Research Unlock Effects
	Button::AddClickEffect("ResearchSpaceport", ResearchSpaceportEffect);
	Button::AddClickEffect("ResearchVehicleDepot", ResearchVehicleDepotEffect);
	Button::AddClickEffect("ResearchTurret", ResearchTurretEffect);
	Button::AddClickEffect("ResearchTeleporter", ResearchTeleporterEffect);

	//Create Unit Effects
	Button::AddClickEffect("CreateUnit1", CreateUnit1Effect);
	Button::AddClickEffect("CreateUnit2", CreateUnit2Effect);
	Button::AddClickEffect("CreateUnit3", CreateUnit3Effect);
	Button::AddClickEffect("CreateUnit4", CreateUnit4Effect);
	*/
}

void ButtonEffects::KillAllUnits(Button &button, float dt, int count, ...)
{
	vector<GameObject*> units = button.GetParent()->GetGameObjectManager()->GetObjectsByName("Unit");
	for (GameObject* go : units)
		go->Destroy();
}

void ButtonEffects::CreateJaxiumMineEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	va_end(args);

	BuildingJaxiumMine *mine;

	try {
		mine = new BuildingJaxiumMine(side, mapPos);
	}
	catch (int) {
		return;
	}

	GameObject *mineObj = new GameObject("Jaxium Mine");
	Transform* transform = new Transform(screenPos.x, screenPos.y);
	transform->SetScale({ 100, 100 });
	mineObj->AddComponent(transform);

	Sprite *sprite = new Sprite();
	mine->texture = AEGfxTextureLoad("Data\\Assets\\Isometric Jaxium Mine Team 1.png");
	SpriteSource* spriteSource = new SpriteSource(1, 1, mine->texture);
	sprite->SetSpriteSource(spriteSource);
	mine->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
	sprite->SetMesh(mine->mesh);

	mineObj->AddComponent(sprite);
	mineObj->AddComponent(mine);

	Space::GetLayer(0)->GetGameObjectManager()->Add(*mineObj);
}

void ButtonEffects::CreateNeoridiumMineEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	va_end(args);

	BuildingNeoridiumMine *mine;

	try {
		mine = new BuildingNeoridiumMine(side, mapPos);
	}
	catch (int) {
		return;
	}

	GameObject *mineObj = new GameObject("Neoridium Mine");
	Transform* transform = new Transform(screenPos.x, screenPos.y);
	transform->SetScale({ 100, 100 });
	mineObj->AddComponent(transform);

	Sprite *sprite = new Sprite();
	mine->texture = AEGfxTextureLoad("");
	SpriteSource* spriteSource = new SpriteSource(1, 1, mine->texture);
sprite->SetSpriteSource(spriteSource);
mine->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
sprite->SetMesh(mine->mesh);

mineObj->AddComponent(sprite);
mineObj->AddComponent(mine);

	Space::GetLayer(0)->GetGameObjectManager()->Add(*mineObj);
}

void ButtonEffects::CreateResearchCenterEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	va_end(args);

	BuildingResearchCenter *researchCenter;

	try {
		researchCenter = new BuildingResearchCenter(side, mapPos);
	}
	catch (int) {
		return;
	}

	GameObject *researchCenterObj = new GameObject("Research Center");
	Transform* transform = new Transform(screenPos.x, screenPos.y);
	transform->SetScale({ 100, 100 });
	researchCenterObj->AddComponent(transform);

	Sprite *sprite = new Sprite();
	researchCenter->texture = AEGfxTextureLoad("Data\\Assets\\Research Center.png");
	SpriteSource* spriteSource = new SpriteSource(1, 1, researchCenter->texture);
	sprite->SetSpriteSource(spriteSource);
	researchCenter->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
	sprite->SetMesh(researchCenter->mesh);

	researchCenterObj->AddComponent(sprite);
	researchCenterObj->AddComponent(researchCenter);

	Space::GetLayer(0)->GetGameObjectManager()->Add(*researchCenterObj);
}

void ButtonEffects::CreateTeleporterEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	va_end(args);

	BuildingTeleporter *teleporter;

	try {
		teleporter = new BuildingTeleporter(side, mapPos);
	}
	catch (int) {
		return;
	}

	GameObject *teleporterObj = new GameObject("Teleporter");
	Transform* transform = new Transform(screenPos.x, screenPos.y);
	transform->SetScale({ 100, 100 });
	teleporterObj->AddComponent(transform);

	Sprite *sprite = new Sprite();
	teleporter->texture = AEGfxTextureLoad("Data\\Assets\\Teleporter.png");
	SpriteSource* spriteSource = new SpriteSource(1, 1, teleporter->texture);
	sprite->SetSpriteSource(spriteSource);
	teleporter->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
	sprite->SetMesh(teleporter->mesh);

	teleporterObj->AddComponent(sprite);
	teleporterObj->AddComponent(teleporter);

	Space::GetLayer(0)->GetGameObjectManager()->Add(*teleporterObj);
}

void ButtonEffects::ResearchSpaceportEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	va_end(args);

	BuildingResearchCenter::Unlock(side, Building::BuildingType::Spaceport);
}

void ButtonEffects::ResearchVehicleDepotEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	va_end(args);

	BuildingResearchCenter::Unlock(side, Building::BuildingType::VehicleDepot);
}

void ButtonEffects::ResearchTurretEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	va_end(args);

	BuildingResearchCenter::Unlock(side, Building::BuildingType::Turret);
}

void ButtonEffects::ResearchTeleporterEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	va_end(args);

	BuildingResearchCenter::Unlock(side, Building::BuildingType::Teleporter);
}

///TODO: Change so that they create the unit at the nearest available position to the command post. (This should still be the cursor position).
void ButtonEffects::CreateUnit1Effect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	BehaviorArmy *army = va_arg(args, BehaviorArmy*);
	va_end(args);

	UNREFERENCED_PARAMETER(side);
	UNREFERENCED_PARAMETER(screenPos);

	vector<GridManager::Node*> nearbyNodes = GridManager::GetInstance().GetNeighbors(GridManager::GetInstance().GetNode((int)mapPos.x, (int)mapPos.y));
	for (unsigned i = 0; i < nearbyNodes.size(); i++) {
		if (!army->LegalSpawn(nearbyNodes[i]->gridPos())) nearbyNodes[i] = nullptr;
	}
	nearbyNodes.erase(std::remove(nearbyNodes.begin(), nearbyNodes.end(), nullptr), nearbyNodes.end());
	if (nearbyNodes.size() == 0) return;

	int nodeID = rand() % nearbyNodes.size();

	army->CreateUnit("Unit1", nearbyNodes[nodeID]->gridPos());
}

void ButtonEffects::CreateUnit2Effect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	BehaviorArmy *army = va_arg(args, BehaviorArmy*);
	va_end(args);

	UNREFERENCED_PARAMETER(side);
	UNREFERENCED_PARAMETER(screenPos);

	vector<GridManager::Node*> nearbyNodes = GridManager::GetInstance().GetNeighbors(GridManager::GetInstance().GetNode((int)mapPos.x, (int)mapPos.y));
	for (unsigned i = 0; i < nearbyNodes.size(); i++) {
		if (!army->LegalSpawn(nearbyNodes[i]->gridPos())) nearbyNodes[i] = nullptr;
	}
	nearbyNodes.erase(std::remove(nearbyNodes.begin(), nearbyNodes.end(), nullptr), nearbyNodes.end());
	if (nearbyNodes.size() == 0) return;

	int nodeID = rand() % nearbyNodes.size();

	army->CreateUnit("Unit2", nearbyNodes[nodeID]->gridPos());
}

void ButtonEffects::CreateUnit3Effect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	BehaviorArmy *army = va_arg(args, BehaviorArmy*);
	va_end(args);

	UNREFERENCED_PARAMETER(side);
	UNREFERENCED_PARAMETER(screenPos);

	vector<GridManager::Node*> nearbyNodes = GridManager::GetInstance().GetNeighbors(GridManager::GetInstance().GetNode((int)mapPos.x, (int)mapPos.y));
	for (unsigned i = 0; i < nearbyNodes.size(); i++) {
		if (!army->LegalSpawn(nearbyNodes[i]->gridPos())) nearbyNodes[i] = nullptr;
	}
	nearbyNodes.erase(std::remove(nearbyNodes.begin(), nearbyNodes.end(), nullptr), nearbyNodes.end());
	if (nearbyNodes.size() == 0) return;

	int nodeID = rand() % nearbyNodes.size();

	army->CreateUnit("Unit3", nearbyNodes[nodeID]->gridPos());
}

void ButtonEffects::CreateUnit4Effect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	BehaviorArmy *army = va_arg(args, BehaviorArmy*);
	va_end(args);

	UNREFERENCED_PARAMETER(side);
	UNREFERENCED_PARAMETER(screenPos);

	vector<GridManager::Node*> nearbyNodes = GridManager::GetInstance().GetNeighbors(GridManager::GetInstance().GetNode((int)mapPos.x, (int)mapPos.y));
	for (unsigned i = 0; i < nearbyNodes.size(); i++) {
		if (!army->LegalSpawn(nearbyNodes[i]->gridPos())) nearbyNodes[i] = nullptr;
	}
	nearbyNodes.erase(std::remove(nearbyNodes.begin(), nearbyNodes.end(), nullptr), nearbyNodes.end());
	if (nearbyNodes.size() == 0) return;

	int nodeID = rand() % nearbyNodes.size();

	army->CreateUnit("Unit4", nearbyNodes[nodeID]->gridPos());
}
