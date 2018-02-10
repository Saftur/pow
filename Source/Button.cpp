//------------------------------------------------------------------------------
//
// File Name:	Button.cpp
// Author(s):	Brand Knutson
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
#include "Button.h"
#include "Vector2D.h"
#include "Transform.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Asteroid) behavior component.
Button::Button() : Component("Button") {

}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

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

		if (mousePos.X() > buttonPos.X() - (buttonScale.X()) && mousePos.X() < buttonPos.X() + (buttonScale.X())
			&& mousePos.Y() > buttonPos.Y() - (buttonScale.Y()) && mousePos.Y() < buttonPos.Y() + (buttonScale.Y())) {
			ClickEffect(dt);
		}
	}
}

//Extra update function called from OnUpdate() that each button can have
//optionally if it needs something to happen every frame.
void Button::OnUpdate(float dt) {
	UNREFERENCED_PARAMETER(dt);
}

template <typename T>
GameObject* Button::CreateButton(const char* objName, Vector2D pos, Vector2D scale, const char* spritePath) {
	GameObject* button = new GameObject(objName);
	Transform* transform = new Transform(pos.X(), pos.Y());
	transform->SetScale(scale);
	button->AddComponent(transform);
	Sprite* sprite = new Sprite();
	sprite->SetMesh(mesh);
	if (spritePath) {
		AEGfxTexture* texture = AEGfxTextureLoad(spritePath);
		textures.push_back(texture);
		SpriteSource* spriteSource = new SpriteSource(1, 1, texture);
		button->AddComponent(spriteSource);
		sprite->SetSpriteSource(spriteSource);
	}
	button->AddComponent(sprite);

	T buttonType = new T();
	button->AddComponent(buttonType);

	buttons.push_back(button);
	return button;
}
