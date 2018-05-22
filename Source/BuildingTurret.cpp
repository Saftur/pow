//------------------------------------------------------------------------------
//
// File Name:	BuildingTurret.cpp
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "BuildingTurret.h"
#include "GameObjectManager.h"
#include "Space.h"
#include "LevelManager.h"
#include "GridManager.h"
#include "PopupMenu.h"
#include <vector>

using std::vector;

BuildingTurret::BuildingTurret(BehaviorArmy::Side side, Vector2D pos) : Building(side, Turret, Special, 3.5f, 250.0f, pos, 50, 0)
{
}

Component * BuildingTurret::Clone() const
{
	return new BuildingTurret(*this);
}

void BuildingTurret::BuildingUpdate(float dt){
	attackTimer -= dt;
	if (!target) FindTarget();
	else if (TargetWithinRange()) AttackTarget();
}

void BuildingTurret::OpenMenu(Vector2D cursorMapPos, Vector2D cursorScreenPos)
{
	PopupMenu::CreateMenu(side, PopupMenu::MenuType::Turret, cursorMapPos, cursorScreenPos);
}

void BuildingTurret::AttackTarget()
{
	FaceTarget();
	if (attackTimer <= 0) {
		attackTimer = 1 / attackSpeed;

		///TODO: Deal damage to the target.
	}
}

void BuildingTurret::FaceTarget()
{
	Transform* myTransform = GetParent()->GetComponent<Transform>();
	Vector2D direction = target->GetComponent<Transform>()->GetTranslation() - myTransform->GetTranslation();
	myTransform->SetRotation(atan2(direction.y, direction.x));
}

void BuildingTurret::FindTarget()
{
	GameObject* closestUnit = nullptr;
	int closestDistance = range + 1;

	for (GameObject* unit : BehaviorUnit::allUnits) {
		BehaviorUnit* behavior = unit->GetComponent<BehaviorUnit>();

		int distance = GridManager::GetInstance().GetDistanceBetween(mapPos, behavior->GetGridPos());
		if (distance < closestDistance) {
			if (behavior->GetArmy()->GetSide() != side) {
				closestDistance = distance;
				closestUnit = unit;
			}
		}
	}
	target = closestUnit;
}

bool BuildingTurret::TargetWithinRange()
{
	bool isWithinRange = GridManager::GetInstance().IsWithinRange(mapPos, target->GetComponent<BehaviorUnit>()->GetGridPos(), range);
	if (!isWithinRange) target = nullptr;
	return isWithinRange;
}
