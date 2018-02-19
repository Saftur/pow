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

//------------------------------------------------------------------------------

map<string, void(*)(float)> Button::clickEffects;

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

void Button::SetEffect(const char * effectName)
{
	ClickEffect = GetClickEffect(effectName);
}

void Button::Load(rapidjson::Value & obj)
{
	UNREFERENCED_PARAMETER(obj);
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
	sprite->SetMeshHalfSize({ 0.5f, 0.5f });
	sprite->SetMeshUV({ 1.0f, 1.0f });
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
	OnUpdate(dt);
	if (AEInputCheckCurr(VK_LBUTTON)) {
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
				ClickEffect(dt);
		}
	}
}

//Extra update function called from OnUpdate() that each button can have
//optionally if it needs something to happen every frame.
void Button::OnUpdate(float dt) {
	UNREFERENCED_PARAMETER(dt);
}

void Button::QuitEffect(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	LevelManager::GetInstance().Quit();
}

void Button::ListEffects()
{
	AddClickEffect("Quit", QuitEffect);
	ButtonEffects::List();
}

void Button::Shutdown() {
	for (unsigned i = 0; i < textures.size(); i++) AEGfxTextureUnload(textures[i]);
	textures.clear();
}

void Button::AddClickEffect(const char * name, void(*effectFunc)(float))
{
	clickEffects[name] = effectFunc;
}

void(*Button::GetClickEffect(const char * name))(float)
{
	return clickEffects.count(name) > 0 ? clickEffects.at(name) : nullptr;
}
