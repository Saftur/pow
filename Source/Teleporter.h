//------------------------------------------------------------------------------
//
// File Name:	Teleporter.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// When a game object goes off-screen, teleport it to the other side.
// Params:
//	 gameObject = Pointer to the game object to be checked.
void TeleporterUpdateObject(GameObject& gameObject);

//------------------------------------------------------------------------------
