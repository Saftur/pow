#include "stdafx.h"
#include "BuildingCommandPost.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "BuildingNeoridiumMine.h"

BuildingCommandPost::BuildingCommandPost(BehaviorArmy::Side side, Vector2D pos) : Building(side, CommandPost, sCommandPost, 0.1f, 1000.0f, pos, 9999999, 9999999)
{

}

Component * BuildingCommandPost::Clone() const
{
	return new BuildingCommandPost(*this);
}

void BuildingCommandPost::BuildingUpdate(float dt)
{
}

void BuildingCommandPost::OpenMenu(Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
	PopupMenu::CreateMenu(side, PopupMenu::MenuType::CommandPost, cursorMapPos, cursorScreenPos);
}
