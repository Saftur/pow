//------------------------------------------------------------------------------
//
// File Name:	BuildingCommandPost.cpp
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BuildingCommandPost.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "BuildingNeoridiumMine.h"
#include "Space.h"
#include "Health.h"

#include "AEEngine.h"

BuildingCommandPost::BuildingCommandPost(BehaviorArmy::Side side, Vector2D pos) : Building(side, CommandPost, Basic, 0.1f, pos, 9999999, 9999999)
{

}

BuildingCommandPost::~BuildingCommandPost()
{
}

void BuildingCommandPost::Load(rapidjson::Value & obj)
{
	SetSide((BehaviorArmy::Side)obj["Side"].GetInt());
	SetPos({ obj["MapPos"][0].GetFloat(), obj["MapPos"][1].GetFloat() });
}

Component * BuildingCommandPost::Clone() const
{
	return new BuildingCommandPost(*this);
}

void BuildingCommandPost::OnDestroy() {
	if (GetParent()->GetComponent<Health>()->GetHP() <= 0) {
		for (unsigned i = 1; i < MAX_LAYERS; i++) Space::DeleteLayer(i);
		if (side == BehaviorArmy::Side::sLeft) Space::GetLayer(0)->GetLevelManager()->SetNextLevel("WinSideRight");
		if (side == BehaviorArmy::Side::sRight) Space::GetLayer(0)->GetLevelManager()->SetNextLevel("WinSideLeft");
	}
}

void BuildingCommandPost::BuildingUpdate(float dt)
{
}

void BuildingCommandPost::OpenMenu()
{
	PopupMenu::CreateMenu(army, PopupMenu::MenuType::CommandPost, GridManager::GetInstance().GetNode(mapPos));
}
