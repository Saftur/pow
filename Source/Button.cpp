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
#include "LevelManager.h"
#include "Button.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Mesh.h"
#include "ButtonEffects.h"
#include "Engine.h"
#include "PopupMenu.h"
#include "BuildingJaxiumMine.h"
#include "SpriteSource.h"

//------------------------------------------------------------------------------

map<string, void(*)(Button&,float, int, ...)> Button::clickEffects;

map<string, Building::BuildingType> Button::buildingType;

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
			if (button.ClickEffect) button.ClickEffect(button, dt, count, side, mapPos, screenPos);
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
	LevelManager::GetLayer(0)->Restart();
	if (LevelManager::GetLayerCount() > 1) {
		PopupMenu::Shutdown();
		for (int i = 1; i < LevelManager::GetLayerCount(); i++) LevelManager::UnloadLayer(i);
	}
}

void Button::QuitEffect(Button &button, float dt, int count, ...)
{
	//LevelManager::GetInstance().Quit();
	Engine::GetInstance().Quit();
}

void Button::CreateMineEffect(Button & button, float dt, int count, ...)
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

	GameObject *mineObj = new GameObject("Mine");
	Transform* transform = new Transform(screenPos.x, screenPos.y);
	transform->SetScale({ 100, 100 });
	mineObj->AddComponent(transform);

	Sprite *sprite = new Sprite();
	sprite->SetModulateColor({ 0, 1, 0, 1 });
	//mine->texture = AEGfxTextureLoad("");
	//SpriteSource* spriteSource = new SpriteSource(1, 1, mine->texture);
	//sprite->SetSpriteSource(spriteSource);
	mine->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
	sprite->SetMesh(mine->mesh);

	mineObj->AddComponent(sprite);
	mineObj->AddComponent(mine);

	LevelManager::GetLayer(0)->GetObjectManager()->Add(*mineObj);
}

void Button::ListEffects()
{
	AddClickEffect("Restart", RestartEffect);
	AddClickEffect("Quit", QuitEffect);
	AddClickEffect("JaxiumMine", CreateMineEffect, Building::BuildingType::JaxiumMine);
	ButtonEffects::List();
}

void Button::Shutdown() {
	for (unsigned i = 0; i < textures.size(); i++) AEGfxTextureUnload(textures[i]);
	textures.clear();
}

void Button::AddClickEffect(const char * name, void(*effectFunc)(Button&,float, int, ...), Building::BuildingType type)
{
	clickEffects[name] = effectFunc;
	buildingType[name] = type;
}

void(*Button::GetClickEffect(const char * name))(Button&,float, int, ...)
{
	return clickEffects.count(name) > 0 ? clickEffects.at(name) : nullptr;
}
