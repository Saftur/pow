//------------------------------------------------------------------------------
//
// File Name:	GameStateDemo.c
// Author(s):	Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <AEEngine.h>
#include "GameObjectManager.h"
#include "GameStateManager.h"
#include "GameStateDemo.h"
#include "Tilemap.h"
#include "Mesh.h"
#include "Trace.h"

#include "Army.h"
#include "BehaviorUnit.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

Tilemap *GameStateDemo::tilemap;
AEGfxTexture *GameStateDemo::textureUnit;
AEGfxTexture *GameStateDemo::textureAbility;
AEGfxVertexList *GameStateDemo::meshUnit;
AEGfxVertexList *GameStateDemo::meshAbility;
SpriteSource *GameStateDemo::spriteSourceUnit;
SpriteSource* GameStateDemo::spriteSourceAbility;

Army *GameStateDemo::army1;
Army *GameStateDemo::army2;

vector<Vector2D> GameStateDemo::path1;
vector<Vector2D> GameStateDemo::path2;

int GameStateDemo::frontLine1;
int GameStateDemo::frontLine2;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Demo game state.
void GameStateDemo::Load()
{
	Trace::GetInstance().GetStream() << "Demo: Load" << std::endl;
	tilemap = new Tilemap("Assets\\Hexidecimal.png", "Data\\Tilemap1.txt", "data\\Tilemap1.collision.txt", 0, 0, 768, 512, 4, 4);
	textureUnit = AEGfxTextureLoad("Assets\\baseunit.png");
	textureAbility = AEGfxTextureLoad("Assets\\abilities.png");
	meshUnit = MeshCreateQuad(32.0f, 32.0f, 1.0f, 1.0f, "Unit Mesh");
	meshAbility = MeshCreateQuad(32.0f, 32.0f, 0.25f, 1.0f, "Ability Mesh");
	spriteSourceUnit = new SpriteSource(1, 1, textureUnit);
	spriteSourceAbility = new SpriteSource(4, 1, textureAbility);
}

// Initialize the memory associated with the Demo game state.
void GameStateDemo::Init()
{
	Trace::GetInstance().GetStream() << "Demo: Init" << std::endl;

	//									  hp, atk, spd
	//Army::Unit *unit1 = new Army::Unit({ 100, 100, 100, Army::Unit::NONE });
	//strcpy(unit1->name, "Unit1");
	//Army::Unit *unit2 = new Army::Unit({ 100, 100, 100, Army::Unit::NONE });
	//strcpy(unit2->name, "Unit1");
	army1 = new Army("Army1");
	//army1->AddUnit(unit1);
	army2 = new Army("Army2");
	//army2->AddUnit(unit2);
	//vector<Vector2D> path;
	path1.push_back({ 1, 0 });
	path1.push_back({ 1, 0 });
	path1.push_back({ 1, 0 });
	path1.push_back({ 1, 0 });
	path1.push_back({ 1, 0 });
	//vector<Vector2D> path2;
	path2.push_back({ -1, 0 });
	path2.push_back({ -1, 0 });
	path2.push_back({ -1, 0 });
	path2.push_back({ -1, 0 });
	path2.push_back({ -1, 0 });
	//CreateUnit(*army1, "Unit1", { 3, 0 }, path1);
	//CreateUnit(*army1, "Unit1", { 2, 0 }, path1);
	//CreateUnit(*army1, "Unit1", { 0, 0 }, path1);
	//CreateUnit(*army1, "Unit1", { 0, 0 }, path);
	//CreateUnit(*army2, "Unit1", { 3, 0 }, path2);
	//CreateUnit(*army2, "Unit1", { 3, 1 }, path2);
	//CreateUnit(*army1, "Unit1", { 0, 1 }, path);
	//CreateUnit(*army1, "Unit1", { 0, 2 }, path);
	//CreateUnit(*army2, "Unit1", { 3, 2 }, path2);
	//CreateUnit(*army2, "Unit1", { 3, 3 }, path2);
	//CreateUnit(*army1, "Unit1", { 0, 3 }, path);

	frontLine1 = 0;
	frontLine2 = 5;
}

// Update the Demo game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateDemo::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Demo: Update" << std::endl;
	
	if (AEInputCheckTriggered('1')) {
		CreateUnit(*army1, "Unit1", { 0, 0 }, path1);
	}
	if (AEInputCheckTriggered('Q')) {
		CreateUnit(*army1, "Unit1", { 0, 1 }, path1);
	}
	if (AEInputCheckTriggered('A')) {
		CreateUnit(*army1, "Unit1", { 0, 2 }, path1);
	}
	if (AEInputCheckTriggered('Z')) {
		CreateUnit(*army1, "Unit1", { 0, 3 }, path1);
	}
	if (AEInputCheckTriggered('7')) {
		CreateUnit(*army2, "Unit1", { 5, 0 }, path2);
	}
	if (AEInputCheckTriggered('U')) {
		CreateUnit(*army2, "Unit1", { 5, 1 }, path2);
	}
	if (AEInputCheckTriggered('J')) {
		CreateUnit(*army2, "Unit1", { 5, 2 }, path2);
	}
	if (AEInputCheckTriggered('M')) {
		CreateUnit(*army2, "Unit1", { 5, 3 }, path2);
	}

	tilemap->Draw();

	if (AEInputCheckTriggered(' '))
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);
}

// Shutdown any memory associated with the Demo game state.
void GameStateDemo::Shutdown()
{
	Trace::GetInstance().GetStream() << "Demo: Shutdown" << std::endl;

	GameObjectManager::GetInstance().Shutdown();

	path1.clear();
	path2.clear();

	delete army1;
	delete army2;
}

// Unload the resources associated with the Demo game state.
void GameStateDemo::Unload()
{
	Trace::GetInstance().GetStream() << "Demo: Unload" << std::endl;
	delete tilemap;
	AEGfxTextureUnload(textureUnit);
	AEGfxMeshFree(meshUnit);
	delete spriteSourceUnit;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void GameStateDemo::CreateUnit(Army &army, const char * name, Vector2D pos, vector<Vector2D> path)
{
	if (!LegalSpawn(army, pos)) return;
	Army::Unit *unit = army.GetUnit(name);
	if (!unit) return;

	GameObject* go = new GameObject("Unit");
	GameObject* overlay = new GameObject("Ability");

	Vector2D screenPos = tilemap->getPosOnScreen(pos);
	Transform* t = new Transform(screenPos.X(), screenPos.Y());
	Transform* abilityT = new Transform(screenPos.X(), screenPos.Y());
	//t->SetScale({});
	go->SetTransform(*t);

	Sprite* sprite = new Sprite("Unit Sprite");
	sprite->SetMesh(meshUnit);
	sprite->SetSpriteSource(spriteSourceUnit);

	Sprite* abilitySprite = new Sprite("Ability Sprite");
	abilitySprite->SetMesh(meshAbility);
	abilitySprite->SetSpriteSource(spriteSourceAbility);
	abilitySprite->SetFrame(unit->ability);

	Physics* p = new Physics();
	Physics* abilityP = new Physics();

	Behavior* b = (Behavior*)new BehaviorUnit(*go, *unit, path, overlay);

	overlay->SetTransform(*abilityT);
	overlay->SetSprite(*abilitySprite);
	overlay->SetPhysics(*abilityP);

	go->SetSprite(*sprite);
	go->SetPhysics(*p);
	go->SetBehavior(*b);

	GameObjectManager::GetInstance().Add(*go);
	GameObjectManager::GetInstance().Add(*overlay);
}

bool GameStateDemo::LegalSpawn(Army & army, Vector2D pos)
{
	vector<GameObject*> units = GameObjectManager::GetInstance().GetObjectsByName("Unit");
	for (GameObject *unit : units) {
		BehaviorUnit *bu = (BehaviorUnit*)(unit->GetBehavior());
		Transform *t = unit->GetTransform();
		if (t) {
			if (pos == tilemap->getPosOnMap(t->GetTranslation()))
				return false;
		}
		if (bu) {
			if (pos == bu->GetNextPos() && bu->IsMoving())
				return false;
		}
	}
	return (&army == army1 && pos.X() <= frontLine1) || (&army == army2 && pos.X() >= frontLine2);
}
