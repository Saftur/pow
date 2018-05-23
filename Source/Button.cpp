//------------------------------------------------------------------------------
//
// File Name:	Button.cpp
// Author(s):	Brand Knutson & Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h" // GameObjectIsNamed, GameObjectDestroy
#include "AEEngine.h" // AEGfxGetWinMaxX, MinX, MaxY, MinY
#include "GameObjectManager.h" // GameObjectManagerAdd
#include "Space.h"
#include "LevelManager.h"
#include "Button.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Mesh.h"
#include "ButtonEffects.h"
#include "Engine.h"
#include "PopupMenu.h"
#include "SpriteSource.h"
#include <algorithm>

//Building includes.
#include "BuildingNeoridiumMine.h"
#include "BuildingJaxiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingTeleporter.h"

//------------------------------------------------------------------------------

map<string, void(*)(Button&,float, int, ...)> Button::clickEffects;

map<string, Building::BuildingType> Button::buildingType;
map<string, BuildingResearchCenter::Research> Button::researchType;

vector<AEGfxTexture*> Button::textures;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Asteroid) behavior component.
Button::Button() : Component("Button") {
	
}

Button::Button(const char * effectName) :
		Button()
{
	SetEffect(effectName);
}

void Button::SetEffect(const char * effectNameA)
{
	ClickEffect = GetClickEffect(effectNameA);
	effectName = effectNameA;
}

void Button::Load(rapidjson::Value & obj)
{
	if (obj.HasMember("ClickEffect") && obj["ClickEffect"].IsString())
		SetEffect(obj["ClickEffect"].GetString());
}

GameObject * Button::CreateButton(const char * objName, const char * effectName, AEGfxVertexList * mesh, Vector2D pos, Vector2D scale, const char * text, Vector2D textScale, Color color, const char * font)
{
	GameObject* button = new GameObject(objName);
	Transform* transform = new Transform(pos.X(), pos.Y());
	transform->SetFollowCamera(false);
	transform->SetScale(scale);
	button->AddComponent(transform);
	Sprite* sprite = new Sprite();
	sprite->SetMesh(mesh);
	//sprite->SetMeshHalfSize({ 0.5f, 0.5f });
	//sprite->SetMeshUV({ 1.0f, 1.0f });
	button->AddComponent(sprite);
	if(text) {
		Text* textObj = new Text(true, text, font, color, textScale);
		button->AddComponent((Component*)textObj);
	}

	Button* buttonComp = new Button(effectName);
	button->AddComponent((Component*)buttonComp);
	return button;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

Component * Button::Clone() const
{
	return new Button(*this);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Button::Update(float dt) {
	if (active) {
		//OnUpdate(dt);
		if (AEInputCheckTriggered(VK_LBUTTON)) {
			//Get the mouse position on screen.
			s32 mouseX;
			s32 mouseY;
			AEInputGetCursorPosition(&mouseX, &mouseY);

			//Convert mouse screen position to world position.
			float worldX;
			float worldY;
			AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &worldX, &worldY);
			Vector2D mousePos = Vector2D(worldX, worldY);

			//Check if the mouse is within the bounds of this button.
			Vector2D buttonScale = ((Transform*)GetParent()->GetComponent("Transform"))->GetScale();
			Vector2D buttonPos = ((Transform*)GetParent()->GetComponent("Transform"))->GetTranslation();

			if (mousePos.X() > buttonPos.X() - (buttonScale.X() / 2) && mousePos.X() <= buttonPos.X() + (buttonScale.X() / 2)
				&& mousePos.Y() > buttonPos.Y() - (buttonScale.Y() / 2) && mousePos.Y() <= buttonPos.Y() + (buttonScale.Y() / 2)) {
				if (ClickEffect)
					ClickEffect(*this, dt, 0);
			}
		}
	}
	else {
		((Sprite*)GetParent()->GetComponent("Sprite"))->SetModulateColor({ 0.6f, 0.6f, 0.4f, 0.5f });
	}
}

void Button::ForceClick(Button& button, float dt, unsigned count, ...) {
	if (button.active) {
		if (count > 0) {
			va_list args;
			va_start(args, count);
			BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
			Vector2D mapPos = va_arg(args, Vector2D);
			Vector2D screenPos = va_arg(args, Vector2D);
			BehaviorArmy *army = va_arg(args, BehaviorArmy*);
			if (button.ClickEffect) button.ClickEffect(button, dt, count, side, mapPos, screenPos, army);
			va_end(args);
		}
		else if (button.ClickEffect) button.ClickEffect(button, dt, 0);
	}
}

//Extra update function called from OnUpdate() that each button can have
//optionally if it needs something to happen every frame.
/*void Button::OnUpdate(float dt) {
	UNREFERENCED_PARAMETER(dt);
}*/

void Button::RestartEffect(Button & button, float dt, int count, ...)
{
	Engine::GetInstance().SetPaused(false);
	Space::GetLayer(0)->GetLevelManager()->Restart();
	if (Space::GetLayerCount() > 1) {
		PopupMenu::Shutdown();
		for (unsigned i = 1; i < Space::GetLayerCount(); i++) Space::DeleteLayer(i);
	}
}

void Button::QuitEffect(Button &button, float dt, int count, ...)
{
	//LevelManager::GetInstance().Quit();
	Engine::GetInstance().Quit();
}

void Button::CreateJaxiumMineEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	va_end(args);

	GameObject mineObj = GameObject(*Space::GetLayer(0)->GetLevelManager()->GetGameObjectManager()->GetArchetype("JaxiumMineArchetype"));
	BuildingJaxiumMine *mine = mineObj.GetComponent<BuildingJaxiumMine>();
	mine->SetPos(mapPos);
	mine->SetSide(side);

	Space::GetLayer(0)->GetGameObjectManager()->Add(mineObj);
}

void Button::CreateNeoridiumMineEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	va_end(args);

	GameObject mineObj = GameObject(*Space::GetLayer(0)->GetLevelManager()->GetGameObjectManager()->GetArchetype("NeoridiumMineArchetype"));
	BuildingNeoridiumMine *mine = mineObj.GetComponent<BuildingNeoridiumMine>();
	mine->SetPos(mapPos);
	mine->SetSide(side);

	Space::GetLayer(0)->GetGameObjectManager()->Add(mineObj);
}

void Button::CreateResearchCenterEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	va_end(args);

	GameObject mineObj = GameObject(*Space::GetLayer(0)->GetLevelManager()->GetGameObjectManager()->GetArchetype("ResearchCenterArchetype"));
	BuildingResearchCenter *mine = mineObj.GetComponent<BuildingResearchCenter>();
	mine->SetPos(mapPos);
	mine->SetSide(side);

	Space::GetLayer(0)->GetGameObjectManager()->Add(mineObj);
}

void Button::CreateTeleporterEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	Vector2D mapPos = va_arg(args, Vector2D);
	Vector2D screenPos = va_arg(args, Vector2D);
	va_end(args);

	GameObject mineObj = GameObject(*Space::GetLayer(0)->GetLevelManager()->GetGameObjectManager()->GetArchetype("TeleporterArchetype"));
	BuildingTeleporter *mine = mineObj.GetComponent<BuildingTeleporter>();
	mine->SetPos(mapPos);
	mine->SetSide(side);

	Space::GetLayer(0)->GetGameObjectManager()->Add(mineObj);
}

void Button::ResearchSpaceportEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	va_end(args);

	BuildingResearchCenter::Unlock(side, Building::BuildingType::Spaceport);
}

void Button::ResearchVehicleDepotEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	va_end(args);

	BuildingResearchCenter::Unlock(side, Building::BuildingType::VehicleDepot);
}

void Button::ResearchTurretEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	va_end(args);

	BuildingResearchCenter::Unlock(side, Building::BuildingType::Turret);
}

void Button::ResearchTeleporterEffect(Button & button, float dt, int count, ...)
{
	va_list args;
	va_start(args, count);
	BehaviorArmy::Side side = va_arg(args, BehaviorArmy::Side);
	va_end(args);

	BuildingResearchCenter::Unlock(side, Building::BuildingType::Teleporter);
}

void Button::CreateUnit1Effect(Button & button, float dt, int count, ...)
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

void Button::CreateUnit2Effect(Button & button, float dt, int count, ...)
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

void Button::CreateUnit3Effect(Button & button, float dt, int count, ...)
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

void Button::CreateUnit4Effect(Button & button, float dt, int count, ...)
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

void Button::ListEffects()
{
	AddClickEffect("Restart", RestartEffect);
	AddClickEffect("Quit", QuitEffect);
	
	//Create Building Effects
	AddClickEffect("CreateJaxiumMine", CreateJaxiumMineEffect, Building::BuildingType::JaxiumMine);
	AddClickEffect("CreateNeoridiumMine", CreateNeoridiumMineEffect, Building::BuildingType::NeoridiumMine);
	AddClickEffect("CreateResearchCenter", CreateResearchCenterEffect, Building::BuildingType::ResearchCenter);
	AddClickEffect("CreateTeleporter", CreateTeleporterEffect, Building::BuildingType::Teleporter);
	///TODO: Implement Click Effects for the remaining buildings.

	//Create Research Unlock Effects
	AddClickEffect("ResearchSpaceport", ResearchSpaceportEffect, Building::BuildingType::Spaceport, BuildingResearchCenter::Research::Spaceport);
	AddClickEffect("ResearchVehicleDepot", ResearchVehicleDepotEffect, Building::BuildingType::VehicleDepot, BuildingResearchCenter::Research::VehicleDepot);
	AddClickEffect("ResearchTurret", ResearchTurretEffect, Building::BuildingType::Turret, BuildingResearchCenter::Research::Turret);
	AddClickEffect("ResearchTeleporter", ResearchTeleporterEffect, Building::BuildingType::Teleporter, BuildingResearchCenter::Research::Teleporter);

	//Create Unit Effects
	AddClickEffect("CreateUnit1", CreateUnit1Effect);
	AddClickEffect("CreateUnit2", CreateUnit2Effect);
	AddClickEffect("CreateUnit3", CreateUnit3Effect);
	AddClickEffect("CreateUnit4", CreateUnit4Effect);
	ButtonEffects::List();
}

void Button::Shutdown() {
	for (unsigned i = 0; i < textures.size(); i++) AEGfxTextureUnload(textures[i]);
	textures.clear();
}

void Button::AddClickEffect(const char * name, void(*effectFunc)(Button&,float, int, ...), Building::BuildingType type, BuildingResearchCenter::Research search)
{
	clickEffects[name] = effectFunc;
	buildingType[name] = type;
	researchType[name] = search;
}

void(*Button::GetClickEffect(const char * name))(Button&,float, int, ...)
{
	return clickEffects.count(name) > 0 ? clickEffects.at(name) : nullptr;
}
