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
// TODO Remove test vars
AEGfxTexture *GameStateDemo::textureHex;
AEGfxVertexList *GameStateDemo::meshQuad;
Sprite *GameStateDemo::spriteHex;
SpriteSource *GameStateDemo::spriteSourceHex;
Transform *GameStateDemo::transformHex;
// End test vars
AEGfxTexture *GameStateDemo::textureUnit;
AEGfxVertexList *GameStateDemo::meshUnit;
SpriteSource *GameStateDemo::spriteSourceUnit;

Army *GameStateDemo::army1;

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
	tilemap = new Tilemap("Assets\\Hexidecimal.png", "Data\\Tilemap1.txt", "data\\Tilemap1.collision.txt", 0, 0, 512, 512);
	// TODO Remove testing
	textureHex = AEGfxTextureLoad("Assets\\Hexidecimal.png");
	meshQuad = MeshCreateQuad(32.0f, 32.0f, 0.25f, 0.25f, "Test Mesh");
	spriteSourceHex = new SpriteSource(4, 4, textureHex);
	spriteHex = new Sprite("Test Sprite");
	spriteHex->SetMesh(meshQuad);
	spriteHex->SetSpriteSource(spriteSourceHex);
	transformHex = new Transform(0, 0);
	// End testing
	textureUnit = AEGfxTextureLoad("Assets\\MonkeyStand.png");
	meshUnit = MeshCreateQuad(32.0f, 32.0f, 1.0f, 1.0f, "Unit Mesh");
	spriteSourceUnit = new SpriteSource(1, 1, textureUnit);
}

// Initialize the memory associated with the Demo game state.
void GameStateDemo::Init()
{
	Trace::GetInstance().GetStream() << "Demo: Init" << std::endl;

	//									  hp, atk, spd
	Army::Unit *unit1 = new Army::Unit({ 100, 100, 100, Army::Unit::NONE });
	strcpy(unit1->name, "Unit1");
	army1 = new Army("Army1");
	army1->AddUnit(unit1);
	vector<Vector2D> path;
	path.push_back({ 1, 0 });
	CreateUnit(*army1, "Unit1", { 0, 0 }, path);
}

// Update the Demo game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateDemo::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Demo: Update" << std::endl;
	
	// TODO Remove testing
	// Get mouse pos in world
	s32 scrMouseX, scrMouseY;
	float mouseX, mouseY;
	AEInputGetCursorPosition(&scrMouseX, &scrMouseY);
	AEGfxConvertScreenCoordinatesToWorld((float)scrMouseX, (float)scrMouseY, &mouseX, &mouseY);

	// Get mouse pos on tilemap
	Vector2D pos = tilemap->getPosOnMap({ mouseX, mouseY });
	// End testing

	tilemap->Draw();

	// TODO Remove testing
	// Draw x, y coordinates under mouse cursor
	spriteHex->SetFrame((int)pos.X());
	transformHex->SetTranslation({ mouseX - 32, mouseY - 32 });
	spriteHex->Draw(*transformHex);
	spriteHex->SetFrame((int)pos.Y());
	transformHex->SetTranslation({ mouseX + 32, mouseY - 32 });
	spriteHex->Draw(*transformHex);
	// End testing

	//GameStateManager::GetInstance().SetNextState(GameStateTable::GsQuit);
}

// Shutdown any memory associated with the Demo game state.
void GameStateDemo::Shutdown()
{
	Trace::GetInstance().GetStream() << "Demo: Shutdown" << std::endl;

	GameObjectManager::GetInstance().Shutdown();

	delete army1;
}

// Unload the resources associated with the Demo game state.
void GameStateDemo::Unload()
{
	Trace::GetInstance().GetStream() << "Demo: Unload" << std::endl;
	delete tilemap;
	// TODO Remove test vars
	delete textureHex;
	AEGfxMeshFree(meshQuad);
	delete spriteSourceHex;
	delete spriteHex;
	delete transformHex;
	// End test vars
	delete textureUnit;
	AEGfxMeshFree(meshUnit);
	delete spriteSourceUnit;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void GameStateDemo::CreateUnit(Army &army, const char * name, Vector2D pos, vector<Vector2D> path)
{
	Army::Unit *unit = army.GetUnit(name);
	if (!name) return;

	GameObject* go = new GameObject("Unit");

	Vector2D screenPos = tilemap->getPosOnScreen(pos);
	Transform* t = new Transform(screenPos.X(), screenPos.Y());
	//t->SetScale({});

	Sprite* sprite = new Sprite("Unit Sprite");
	sprite->SetMesh(meshUnit);
	sprite->SetSpriteSource(spriteSourceUnit);

	//Collider* c = new Collider(*go);

	Physics* p = new Physics();

	Behavior* b = (Behavior*)new BehaviorUnit(*go, *unit, path);

	go->SetTransform(*t);
	go->SetSprite(*sprite);
	go->SetPhysics(*p);
	go->SetBehavior(*b);
	//go->SetCollider(*c);

	GameObjectManager::GetInstance().Add(*go);
}