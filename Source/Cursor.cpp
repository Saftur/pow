//------------------------------------------------------------------------------
//
// File Name:	Cursor.cpp
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
#include "Cursor.h"
#include <AEEngine.h>
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Controls.h"
#include "Tilemap.h"

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

// Allocate a new (Cursor) behavior component.
// Params:
//  parent = The object that owns this behavior.
Cursor::Cursor() : Component("Cursor") {
}

void Cursor::Load(rapidjson::Value & obj) {
	rapidjson::Value *tmp;
	if (obj.HasMember("MoveSpeed") && (tmp = &obj["MoveSpeed"])->IsNumber())
		moveSpeed = tmp->GetFloat();
}

void Cursor::PostLoadInit() {
	transform = GetParent()->GetComponent<Transform>();
	tilemap = GetParent()->GetGameObjectManager()->GetObjectByName("Tilemap")->GetComponent<Tilemap>();
}

void Cursor::Update(float dt) {
	if (!gamepad) return;
	Vector2D pos = transform->GetTranslation();
	Vector2D scl = transform->GetScale();

	Vector2D move = {gamepad->GetAxis(AXS_CURSOR_X), gamepad->GetAxis(AXS_CURSOR_Y)};

	//Temporary keyboard controls.
	if (AEInputCheckCurr(VK_UP)) move.y += 0.5;
	if (AEInputCheckCurr(VK_DOWN)) move.y -= 0.5;
	if (AEInputCheckCurr(VK_LEFT)) move.x -= 0.5;
	if (AEInputCheckCurr(VK_RIGHT)) move.x += 0.5;

	pos += move * moveSpeed * dt;


	if (pos.x + scl.x / 2 > AEGfxGetWinMaxX())
		pos.x = AEGfxGetWinMaxX() - scl.x / 2;
	if (pos.x - scl.x / 2 < AEGfxGetWinMinX())
		pos.x = AEGfxGetWinMinX() + scl.x / 2;
	if (pos.y + scl.y / 2 > AEGfxGetWinMaxY())
		pos.y = AEGfxGetWinMaxY() - scl.y / 2;
	if (pos.y - scl.y / 2 < AEGfxGetWinMinY())
		pos.y = AEGfxGetWinMinY() + scl.y / 2;

	transform->SetTranslation(pos);
}

void Cursor::SetGamepad(Gamepad * gp) {
	gamepad = gp;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* Cursor::Clone() const {
	return new Cursor(*this);
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
