//------------------------------------------------------------------------------
//
// File Name:	MenuCursor.cpp
// Author(s):	Arthur Bouvier
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
#include "MenuCursor.h"
#include <AEEngine.h>
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Controls.h"
#include "Button.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (MenuCursor) behavior component.
// Params:
//  parent = The object that owns this behavior.
MenuCursor::MenuCursor() : Component("MenuCursor") {
}

void MenuCursor::Load(rapidjson::Value & obj) {
	rapidjson::Value *tmp;

	if (obj.HasMember("GamepadNum") && (tmp = &obj["GamepadNum"])->IsUint())
		gamepad = &Gamepad::GetGamepad(tmp->GetUint());
	if (obj.HasMember("ConeAngle") && (tmp = &obj["ConeAngle"])->IsNumber())
		coneAngle = tmp->GetFloat();
	if (obj.HasMember("OffScale") && (tmp = &obj["OffScale"])->IsNumber())
		offScale = tmp->GetFloat();
}

void MenuCursor::InstanceInit() {
	transform = GetParent()->GetComponent<Transform>();
}

void MenuCursor::Update(float dt) {
	if (!gamepad) return;
	Vector2D moveDir(0, 0);
	if (gamepad->GetButtonTriggered(BTN_MENU_LEFT))
		moveDir.x = -1;
	else if (gamepad->GetButtonTriggered(BTN_MENU_RIGHT))
		moveDir.x = 1;
	else if (gamepad->GetButtonTriggered(BTN_MENU_UP))
		moveDir.y = 1;
	else if (gamepad->GetButtonTriggered(BTN_MENU_DOWN))
		moveDir.y = -1;

	if (!selectedButton)
		moveDir.y = -1;

	if (moveDir.x != 0 || moveDir.y != 0) {
		float moveAngle = moveDir.GetAngleDegrees();
		Vector2D currentPos = transform->GetTranslation();//selectedButton ? selectedButton->GetComponent<Transform>()->GetTranslation() : ;
		float closestDist = 0;
		GameObject *button = nullptr;
		Transform *buttonTrs;
		GetParent()->GetGameObjectManager()->ForEachObject([&](GameObject *obj) {
			if (!obj->GetChildComponent<Button>() || obj == selectedButton)
				return;
			Transform *objTrs = obj->GetComponent<Transform>();
			Vector2D objPos = objTrs->GetTranslation();
			Vector2D diff = (objPos - currentPos);
			float angle = diff.GetAngleDegrees();
			float angleDiff = abs(angle-moveAngle);
			if (angleDiff > coneAngle)
				return;
			float endDist = (diff.Magnitude() * (1.f + (angleDiff / coneAngle * offScale)));
			if (closestDist == 0 || (endDist < closestDist)) {
				button = obj;
				closestDist = endDist;
				buttonTrs = objTrs;
			}
		});
		if (button) {
			selectedButton = button;
			transform->SetTranslation(buttonTrs->GetTranslation());
			transform->SetScale(buttonTrs->GetScale() * 1.1f);
		}
	}

	if (gamepad->GetButtonTriggered(BTN_MENU_SELECT) && selectedButton)
		selectedButton->GetChildComponent<Button>()->Click(dt);
}

void MenuCursor::SetGamepad(Gamepad * gp) {
	gamepad = gp;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* MenuCursor::Clone() const {
	return new MenuCursor(*this);
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
