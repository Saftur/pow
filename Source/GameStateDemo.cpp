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
#include "GameStateManager.h"
#include "GameStateDemo.h"
#include "Tilemap.h"
#include "Mesh.h"
#include "Trace.h"

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
}

// Initialize the memory associated with the Demo game state.
void GameStateDemo::Init()
{
	Trace::GetInstance().GetStream() << "Demo: Init" << std::endl;
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
}

// Unload the resources associated with the Demo game state.
void GameStateDemo::Unload()
{
	Trace::GetInstance().GetStream() << "Demo: Unload" << std::endl;
	delete tilemap;
	// TODO Remove test vars
	delete textureHex;
	delete meshQuad;
	delete spriteSourceHex;
	delete spriteHex;
	delete transformHex;
	// End test vars
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

