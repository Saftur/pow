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

Crystal::~Crystal()
{
	if(mesh) {
		AEGfxMeshFree(mesh);
		AEGfxTextureUnload(texture);
		mesh = nullptr;
		texture = nullptr;
	}
}

Component * Crystal::Clone() const
{
	return new Crystal(*this);
}

void Crystal::Update(float dt)
{
	///TODO: If a unit moves over this crystal, add either Jaxium or Neoridium to their army.
	Space::GetLayer(0)->GetGameObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
		if (obj->GetComponent<BehaviorUnit>()) {
			if (GridManager::GetInstance().ConvertToGridPoint(obj->GetComponent<Transform>()->GetTranslation())
				== GridManager::GetInstance().ConvertToGridPoint(GetParent()->GetComponent<Transform>()->GetTranslation())) {

				BehaviorArmy* army = obj->GetComponent<BehaviorUnit>()->GetArmy();
				if (type == Jaxium) army->AddToFunds(crystalCount);
				else if (type == Neoridium) BuildingNeoridiumMine::AddNeoridium(army->GetSide(), crystalCount);
				GetParent()->Destroy();
			}
		}
		return false;
	});
}
