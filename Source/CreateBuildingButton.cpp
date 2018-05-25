#include "stdafx.h"
#include "CreateBuildingButton.h"

#include <AEEngine.h>
#include "Space.h"

// TODO Remove when switched to archetypes
#include "SpriteSource.h"
#include "Mesh.h"
// END TODO

#include "BuildingJaxiumMine.h"
#include "BuildingNeoridiumMine.h"
#include "BuildingResearchCenter.h"
#include "BuildingTeleporter.h"

CreateBuildingButton::CreateBuildingButton() : ArmyButton("CreateBuildingButton") {
}

void CreateBuildingButton::Load(rapidjson::Value & obj) {
	ArmyButton::Load(obj);
	rapidjson::Value *tmp;

	if (obj.HasMember("BuildingArchetype") && (tmp = &obj["BuildingArchetype"])->IsString()) {
		archetypeName = tmp->GetString();
		buildingArchetype = Space::GetLayer(0)->GetGameObjectManager()->GetArchetype(archetypeName.c_str());
	}
}

Component * CreateBuildingButton::Clone() const {
	return new CreateBuildingButton(*this);
}

void CreateBuildingButton::InstanceInit() {
	if (buildingArchetype) {
		Building::BuildingType buildingType = buildingArchetype->GetChildComponent<Building>()->buildingType;
		SetActive(Building::CanBuy(army, buildingType));
	}
}

void CreateBuildingButton::Update(float dt) {
	if (buildingArchetype) {
		Building::BuildingType buildingType = buildingArchetype->GetChildComponent<Building>()->buildingType;
		SetActive(Building::CanBuy(army, buildingType));
	}
}

void CreateBuildingButton::ClickEffect(float dt) {
	GameObject *newBuilding = new GameObject(*buildingArchetype);
	Building *building = newBuilding->GetChildComponent<Building>();
	building->SetPos(mapPos->gridPos());
	building->SetArmy(army);
	building->Buy();
	Space::GetLayer(0)->GetGameObjectManager()->Add(*newBuilding);
	/*if (archetypeName == "JaxiumMineArchetype") {
		BuildingJaxiumMine *mine;

		try {
			mine = new BuildingJaxiumMine(army->GetSide(), mapPos->gridPos());
		} catch (int) {
			return;
		}

		GameObject *mineObj = new GameObject("Jaxium Mine");
		Transform* transform = new Transform(GridManager::GetInstance().ConvertToWorldPoint(mapPos));
		transform->SetScale({100, 100});
		mineObj->AddComponent(transform);

		Sprite *sprite = new Sprite();
		mine->texture = AEGfxTextureLoad("Data\\Assets\\Isometric Jaxium Mine Team 1.png");
		SpriteSource* spriteSource = new SpriteSource(1, 1, mine->texture);
		sprite->SetSpriteSource(spriteSource);
		mine->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
		sprite->SetMesh(mine->mesh);

		mineObj->AddComponent(sprite);
		mineObj->AddComponent(mine);

		Space::GetLayer(0)->GetGameObjectManager()->Add(*mineObj);
	}

	else if (archetypeName == "NeoridiumMineArchetype") {
		BuildingNeoridiumMine *mine;

		try {
			mine = new BuildingNeoridiumMine(army->GetSide(), mapPos->gridPos());
		} catch (int) {
			return;
		}

		GameObject *mineObj = new GameObject("Neoridium Mine");
		Transform* transform = new Transform(GridManager::GetInstance().ConvertToWorldPoint(mapPos));
		transform->SetScale({100, 100});
		mineObj->AddComponent(transform);

		Sprite *sprite = new Sprite();
		mine->texture = AEGfxTextureLoad("");
		SpriteSource* spriteSource = new SpriteSource(1, 1, mine->texture);
		sprite->SetSpriteSource(spriteSource);
		mine->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
		sprite->SetMesh(mine->mesh);

		mineObj->AddComponent(sprite);
		mineObj->AddComponent(mine);

		Space::GetLayer(0)->GetGameObjectManager()->Add(*mineObj);
	}

	else if (archetypeName == "ResearchCenterArchetype") {
		BuildingResearchCenter *researchCenter;

		try {
			researchCenter = new BuildingResearchCenter(army->GetSide(), mapPos->gridPos());
		} catch (int) {
			return;
		}

		GameObject *researchCenterObj = new GameObject("Research Center");
		Transform* transform = new Transform(GridManager::GetInstance().ConvertToWorldPoint(mapPos));
		transform->SetScale({100, 100});
		researchCenterObj->AddComponent(transform);

		Sprite *sprite = new Sprite();
		researchCenter->texture = AEGfxTextureLoad("Data\\Assets\\Research Center.png");
		SpriteSource* spriteSource = new SpriteSource(1, 1, researchCenter->texture);
		sprite->SetSpriteSource(spriteSource);
		researchCenter->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
		sprite->SetMesh(researchCenter->mesh);

		researchCenterObj->AddComponent(sprite);
		researchCenterObj->AddComponent(researchCenter);

		Space::GetLayer(0)->GetGameObjectManager()->Add(*researchCenterObj);
	}

	else if (archetypeName == "TeleporterArchetype") {
		BuildingTeleporter *teleporter;

		try {
			teleporter = new BuildingTeleporter(army->GetSide(), mapPos->gridPos());
		} catch (int) {
			return;
		}

		GameObject *teleporterObj = new GameObject("Teleporter");
		Transform* transform = new Transform(GridManager::GetInstance().ConvertToWorldPoint(mapPos));
		transform->SetScale({100, 100});
		teleporterObj->AddComponent(transform);

		Sprite *sprite = new Sprite();
		teleporter->texture = AEGfxTextureLoad("Data\\Assets\\Teleporter.png");
		SpriteSource* spriteSource = new SpriteSource(1, 1, teleporter->texture);
		sprite->SetSpriteSource(spriteSource);
		teleporter->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
		sprite->SetMesh(teleporter->mesh);

		teleporterObj->AddComponent(sprite);
		teleporterObj->AddComponent(teleporter);

		Space::GetLayer(0)->GetGameObjectManager()->Add(*teleporterObj);
	}*/
}
