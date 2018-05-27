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
#include "Button.h"
#include "GameObject.h" // GameObjectIsNamed, GameObjectDestroy
#include "AEEngine.h" // AEGfxGetWinMaxX, MinX, MaxY, MinY
#include "GameObjectManager.h" // GameObjectManagerAdd
#include "Space.h"
#include "LevelManager.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Mesh.h"
#include "ButtonEffects.h"
#include "Engine.h"
#include "SpriteSource.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Asteroid) behavior component.
Button::Button(const char *name) : Component(name) {
	
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

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
				Click(dt);
			}
		}
	}
}

void Button::Click(float dt) {
	if (active)
		ClickEffect(dt);
}

void Button::SetActive(bool isActive) {
	if (active && !isActive)
		((Sprite*)GetParent()->GetComponent("Sprite"))->SetModulateColor({ 0.6f, 0.6f, 0.4f, 0.5f });
	else if (!active && isActive)
		((Sprite*)GetParent()->GetComponent("Sprite"))->SetModulateColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	active = isActive;
}

bool Button::GetActive() const {
	return active;
}
