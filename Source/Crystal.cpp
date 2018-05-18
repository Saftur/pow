#include "stdafx.h"
#include "AEEngine.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "Crystal.h"
#include "Space.h"
#include "GameObjectManager.h"
#include "LevelManager.h"
#include "GridManager.h"
#include "Transform.h"
#include "BehaviorUnit.h"
#include "GameObject.h"
#include "BehaviorArmy.h"
#include "BuildingNeoridiumMine.h"

Crystal::Crystal(CrystalType type, float amount) : Component("Crystal"), type(type), crystalCount(amount)
{
}

Component * Crystal::Clone() const
{
	return new Crystal(*this);
}

void Crystal::Update(float dt)
{
	//Give crystals to the unit that walks ontop of it.
	GameObject* unit = GridManager::GetInstance().GetOccupant(GridManager::GetInstance().ConvertToGridPoint(GetParent()->GetComponent<Transform>()->GetTranslation()));
	if (unit) {
		BehaviorArmy* army = unit->GetComponent<BehaviorUnit>()->GetArmy();
		if (type == Jaxium) army->AddToFunds(crystalCount);
		else if (type == Neoridium) BuildingNeoridiumMine::AddNeoridium(army->GetSide(), crystalCount);
		GetParent()->Destroy();
	}
}
