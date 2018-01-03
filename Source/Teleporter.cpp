//------------------------------------------------------------------------------
//
// File Name:	Teleporter.cpp
// Author(s):	Mark Culp
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
#include "AEEngine.h"
#include "Teleporter.h"
#include "GameObject.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// When a game object goes off-screen, teleport it to the other side.
// Params:
//	 gameObject = Reference to the game object to be checked.
void TeleporterUpdateObject(GameObject& gameObject)
{
	Vector2D pos = gameObject.GetTransform()->GetTranslation();

	if (gameObject.GetTransform()->GetTranslation().X() >= AEGfxGetWinMaxX())
		pos.X(AEGfxGetWinMinX());
	if (gameObject.GetTransform()->GetTranslation().Y() >= AEGfxGetWinMaxY())
		pos.Y(AEGfxGetWinMinY());
	if (gameObject.GetTransform()->GetTranslation().X() <= AEGfxGetWinMinX())
		pos.X(AEGfxGetWinMaxX());
	if (gameObject.GetTransform()->GetTranslation().Y() <= AEGfxGetWinMinY())
		pos.Y(AEGfxGetWinMaxY());

	gameObject.GetTransform()->SetTranslation(pos);
}

//------------------------------------------------------------------------------
