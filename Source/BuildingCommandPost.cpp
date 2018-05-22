#include "stdafx.h"
#include "BuildingCommandPost.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "BuildingNeoridiumMine.h"
#include "Space.h"

#include "AEEngine.h"

bool BuildingCommandPost::gameOver = false;

BuildingCommandPost::BuildingCommandPost(BehaviorArmy::Side side, Vector2D pos) : Building(side, CommandPost, sCommandPost, 0.1f, 1000.0f, pos, 9999999, 9999999)
{

}

BuildingCommandPost::~BuildingCommandPost()
{
	if (!gameOver) {
		gameOver = true;
		for (unsigned i = 1; i < MAX_LAYERS; i++) Space::DeleteLayer(i);
		if (side == BehaviorArmy::Side::sLeft) Space::GetLayer(0)->GetLevelManager()->SetNextLevel("WinSideLeft");
		if (side == BehaviorArmy::Side::sRight) Space::GetLayer(0)->GetLevelManager()->SetNextLevel("WinSideRight");
	}
}

Component * BuildingCommandPost::Clone() const
{
	return new BuildingCommandPost(*this);
}

void BuildingCommandPost::BuildingUpdate(float dt)
{
	if (AEInputCheckTriggered('V') && side == BehaviorArmy::Side::sLeft) GetParent()->Destroy();
}

void BuildingCommandPost::OpenMenu(Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
	PopupMenu::CreateMenu(side, PopupMenu::MenuType::CommandPost, cursorMapPos, cursorScreenPos);
}
