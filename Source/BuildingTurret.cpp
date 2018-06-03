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
#include "BehaviorProjectile.h"
#include <vector>

using std::vector;

BuildingTurret::BuildingTurret(BehaviorArmy::Side side, Vector2D pos) : Building(side, Turret, Special, 3.5f, pos, 50, 0)
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

void BuildingTurret::OpenMenu()
{
	PopupMenu::CreateMenu(army, PopupMenu::MenuType::Turret, GridManager::GetInstance().GetNode(mapPos));
}

void BuildingTurret::Upgrade(UpgradeType type)
{
	upgradeCost *= 1.5;
	switch (type) {
	case Damage:
		damage += 25;
		break;
	case Range:
		range++;
		break;
	case AttackSpeed:
		attackSpeed++;
		break;
	}
}

void BuildingTurret::AttackTarget()
{
	FaceTarget();
	if (attackTimer <= 0) {
		attackTimer = 1 / attackSpeed;

		GameObject *projectile = new GameObject(*BehaviorProjectile::Projectiles[BehaviorProjectile::ProjectileTypes::pTypeLaser]);
		projectile->GetComponent<Transform>()->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(mapPos));
		projectile->GetComponent<BehaviorProjectile>()->Fire(army, targetDirection, (int)damage, range);
		Space::GetLayer(0)->GetGameObjectManager()->Add(*projectile);
	}
}

void BuildingTurret::FaceTarget()
{
	Transform* myTransform = GetParent()->GetComponent<Transform>();
	targetDirection = target->GetComponent<Transform>()->GetTranslation() - myTransform->GetTranslation();
	float rot = atan2(targetDirection.y, targetDirection.x);
	myTransform->SetRotation(rot);
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
	if (target->IsDestroyed()) {
		target = nullptr;
		return false;
	}

	bool isWithinRange = GridManager::GetInstance().IsWithinRange(mapPos, target->GetComponent<BehaviorUnit>()->GetGridPos(), range);
	if (!isWithinRange) target = nullptr;
	return isWithinRange;
}
