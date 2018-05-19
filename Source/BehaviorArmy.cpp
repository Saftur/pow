//------------------------------------------------------------------------------
//
// File Name:	BehaviorArmy.cpp
// Author(s):	Arthur Bouvier
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
#include "Random.h"
#include "AEEngine.h"
#include "Teleporter.h"
#include "BehaviorArmy.h"
#include "Space.h"
#include "GameObjectManager.h"
#include "Cursor.h"
#include "BehaviorUnit.h"
#include "Transform.h"
#include "Trace.h"
#include "Engine.h"
#include "PopupMenu.h"
#include "Building.h"
#include "BuildingResearchCenter.h"
#include "BuildingCommandPost.h"
#include "Controls.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include <iostream>
#include <sstream>
#include "LevelManager.h"
using std::ifstream;
using std::stringstream;

#include "BuildingTurret.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states { cArmyNormal };

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Army) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorArmy::BehaviorArmy() :
		Behavior("BehaviorArmy")
{
	SetCurrentState(cBehaviorInvalid);
	SetNextState(cArmyNormal);
}

void BehaviorArmy::PushFrontLine(Vector2D pos)
{
	vector<GameObject*> unitGOs = GetParent()->GetGameObjectManager()->GetObjectsByName("Unit");
	vector<GameObject*> armyGOs = GetParent()->GetGameObjectManager()->GetObjectsByName("Army");
	switch (side) {
	case sLeft:
		if (pos.x > frontLine.pos) {
			frontLine.pos = (int)pos.x;
			for (GameObject *unit : unitGOs) {
				Transform *t = unit->GetComponent<Transform>();
				BehaviorUnit *bu = unit->GetComponent<BehaviorUnit>();
				if (!t || !bu) continue;
				Vector2D mapPos = tilemap->GetPosOnMap(t->GetTranslation());
				if (bu->GetArmy()->GetSide() != side && mapPos.x <= frontLine.pos) {
					frontLine.pos = (int)mapPos.x - 1;
				}
			}
		}
		if (frontLine.pos < 0) frontLine.pos = 0;
		if (frontLine.pos >= tilemap->GetTilemapWidth() - 1) frontLine.pos = tilemap->GetTilemapWidth() - 2;
		if (frontLine.transform)
			frontLine.transform->SetTranslation(tilemap->GetPosOnScreen({ (float)frontLine.pos, (float)(tilemap->GetTilemapHeight()) / 2 }) + 
										Vector2D((float)(tilemap->GetTileWidth())/2, (float)(tilemap->GetTileHeight())/2));
		break;
	case sRight:
		if (pos.x < frontLine.pos) {
			frontLine.pos = (int)pos.x;
			for (GameObject *unit : unitGOs) {
				Transform *t = unit->GetComponent<Transform>();
				BehaviorUnit *bu = unit->GetComponent<BehaviorUnit>();
				if (!t || !bu) continue;
				Vector2D mapPos = tilemap->GetPosOnMap(t->GetTranslation());
				if (bu->GetArmy()->GetSide() != side && mapPos.x >= frontLine.pos) {
					frontLine.pos = (int)mapPos.x + 1;
				}
			}
		}
		if (frontLine.pos >= tilemap->GetTilemapWidth()) frontLine.pos = tilemap->GetTilemapWidth()-1;
		if (frontLine.pos <= 0) frontLine.pos = 1;
		if (frontLine.transform)
			frontLine.transform->SetTranslation(tilemap->GetPosOnScreen({ (float)frontLine.pos, (float)(tilemap->GetTilemapHeight()) / 2 }) + 
										Vector2D(-(float)(tilemap->GetTileWidth())/2, (float)(tilemap->GetTileHeight())/2));
		break;
	}
	for (GameObject *army : armyGOs) {
		BehaviorArmy *ba = army->GetComponent<BehaviorArmy>();
		if (!ba || ba->frontLine.pos < 0) continue;
		if (ba->GetSide() != side) {
			switch (ba->GetSide()) {
			case sLeft:
				if (pos.x <= ba->frontLine.pos) {
					ba->frontLine.pos = (int)pos.x - 1;
					if (ba->frontLine.pos < 0) ba->frontLine.pos = 0;
				}
				if (ba->frontLine.transform)
					ba->frontLine.transform->SetTranslation(tilemap->GetPosOnScreen({ (float)ba->frontLine.pos, (float)(tilemap->GetTilemapHeight()) / 2 }) + 
												Vector2D((float)(tilemap->GetTileWidth())/2, (float)(tilemap->GetTileHeight())/2));
				break;
			case sRight:
				if (pos.x >= ba->frontLine.pos) {
					ba->frontLine.pos = (int)pos.x + 1;
					if (ba->frontLine.pos >= tilemap->GetTilemapWidth()) ba->frontLine.pos = tilemap->GetTilemapWidth()-1;
				}
				if (ba->frontLine.transform)
					ba->frontLine.transform->SetTranslation(tilemap->GetPosOnScreen({ (float)ba->frontLine.pos, (float)(tilemap->GetTilemapHeight()) / 2 }) + 
												Vector2D(-(float)(tilemap->GetTileWidth())/2, (float)(tilemap->GetTileHeight())/2));
				break;
			}
		}
	}
}

BehaviorArmy::Side BehaviorArmy::GetSide()
{
	return side;
}

void BehaviorArmy::IncreaseUnits()
{
	numUnits++;
}

void BehaviorArmy::DescreaseUnits()
{
	numUnits--;
}

float BehaviorArmy::GetFunds()
{
	return funds.amount;
}

bool BehaviorArmy::TakeFromFunds(float amount)
{
	if (funds.amount >= amount) {
		funds.amount -= amount;
		UpdateFundsText();
		return true;
	} else {
		return false;
	}
}

void BehaviorArmy::AddToFunds(float amount)
{
	funds.amount += amount;
	UpdateFundsText();
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* BehaviorArmy::Clone() const
{
	return new BehaviorArmy(*this);
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorArmy::OnEnter()
{
	switch (GetCurrentState())
	{
	case cArmyNormal:
		// Initialize tilemap
		tilemap = GetParent()->GetGameObjectManager()->GetObjectByName("Tilemap")->GetComponent<Tilemap>();

		// Initialize the front line struct.
		GameObject *frontLineObj = GetParent()->GetGameObjectManager()->GetObjectByName(frontLine.dispObjName.c_str());
		
		if (frontLineObj) 
			frontLine.transform = frontLineObj->GetComponent<Transform>();
		else 
			frontLine.transform = nullptr;

		// Initialize the funds struct.
		GameObject *fundsTextObj = GetParent()->GetGameObjectManager()->GetObjectByName(funds.dispObjName.c_str());

		if (fundsTextObj) 
			funds.text = fundsTextObj->GetComponent<Text>();
		else 
			funds.text = nullptr;

		funds.amount = (float)funds.startAmount;
		UpdateFundsText();

		// Initialize the cursor struct.
		GameObject *cursorObj = GetParent()->GetGameObjectManager()->GetObjectByName(cursor.objName.c_str());

		cursor.transform = cursorObj->GetComponent<Transform>();
		cursorObj->GetComponent<Cursor>()->SetGamepad(controls.gamepad);
		cursor.sprite = cursorObj->GetComponent<Sprite>();

		// Initialize the path struct.
		GameObject *pathArchetype = GetParent()->GetGameObjectManager()->GetArchetype(path.lineDispName.c_str());

		if (pathArchetype) 
		{
			path.sprite = pathArchetype->GetComponent<Sprite>();
			path.transform = pathArchetype->GetComponent<Transform>();
		}

		// Initialize the front line and cursor positions.
		switch (side) 
		{
		case sLeft:
			frontLine.start = frontLine.start < 0 ? 0 : frontLine.start;
			cursor.transform->SetTranslation(tilemap->GetPosOnScreen({ 0, 0 }));
			break;
		case sRight:
			frontLine.start = frontLine.start < 0 ? tilemap->GetTilemapWidth() - 1 : frontLine.start;
			cursor.transform->SetTranslation(tilemap->GetPosOnScreen({ (float)tilemap->GetTilemapWidth() - 1, 0 }));
			break;
		}
		frontLine.pos = frontLine.start;
		if (frontLine.transform) {
			PushFrontLine({ (float)frontLine.pos, 0 });
		}

		Vector2D pos;
		if (side == sLeft) pos = GridManager::GetInstance().ConvertToWorldPoint(GridManager::GetInstance().GetNode(2, 4));
		if (side == sRight) pos = GridManager::GetInstance().ConvertToWorldPoint(GridManager::GetInstance().GetNode(10, 4));

		BuildingCommandPost *commandPost;
		commandPost = new BuildingCommandPost(side, pos);

		GameObject *commandPostObj = new GameObject("Command Post");
		Transform* transform = new Transform(pos.x, pos.y);
		transform->SetScale({ 50, 50 });
		commandPostObj->AddComponent(transform);

		Sprite *sprite = new Sprite();
		commandPost->texture = AEGfxTextureLoad("Data\\Assets\\Command Post.png");
		SpriteSource* spriteSource = new SpriteSource(1, 1, commandPost->texture);
		sprite->SetSpriteSource(spriteSource);
		commandPost->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
		sprite->SetMesh(commandPost->mesh);

		commandPostObj->AddComponent(sprite);
		commandPostObj->AddComponent(commandPost);

		Space::GetLayer(0)->GetGameObjectManager()->Add(*commandPostObj);

		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorArmy::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	switch (GetCurrentState())
	{
	case cArmyNormal:
		// Get current cursor position
		Vector2D cursPos = cursor.transform->GetWorldTranslation();
		// Tilemap top left point
		Vector2D tilemapTopLeft = tilemap->GetTilemapScreenTopLeft();

		// Tilemap bottom right point
		Vector2D tilemapBottomRight = tilemap->GetTilemapScreenBottomRight();

		// Cursor Node position
		Node* cursorNode = GridManager::GetInstance().GetNode(GridManager::GetInstance().ConvertToGridPoint(cursPos));

		// If any extra selected units are destroyed, deselect them
		for (unsigned i = 0; i < selectedUnits.size(); i++) {
			if (selectedUnits[i].unit->GetParent()->IsDestroyed()) {
				selectedUnits.erase(selectedUnits.begin() + i);
				i--;
			}
		}

		///TODO: Remove both of these if statements and everything they contain.
		if (controls.gamepad->GetButtonTriggered(Gamepad::bDpadDown)) {
			Vector2D mapPos = GridManager::GetInstance().ConvertToGridPoint(cursor.transform->GetTranslation());
			Vector2D screenPos = cursor.transform->GetTranslation();

			BuildingTurret *mine;

			try {
				mine = new BuildingTurret(side, mapPos);
			}
			catch (int) {
				return;
			}

			GameObject *mineObj = new GameObject("Turret");
			Transform* transform = new Transform(screenPos.x, screenPos.y);
			transform->SetScale({ 100, 100 });
			mineObj->AddComponent(transform);

			Sprite *sprite = new Sprite();
			mine->texture = AEGfxTextureLoad("Data\\Assets\\Turret.png");
			SpriteSource* spriteSource = new SpriteSource(1, 1, mine->texture);
			sprite->SetSpriteSource(spriteSource);
			mine->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
			sprite->SetMesh(mine->mesh);

			mineObj->AddComponent(sprite);
			mineObj->AddComponent(mine);

			Space::GetLayer(0)->GetGameObjectManager()->Add(*mineObj);
		}
		if (controls.gamepad->GetButtonTriggered(Gamepad::bDpadUp)) {
			Vector2D screenPos = cursor.transform->GetTranslation();
			screenPos.y -= 250;

			for (unsigned i = 0; i < 100; i++) {
				Vector2D mapPos = GridManager::GetInstance().ConvertToGridPoint(screenPos);
				screenPos.y += 5;

				BuildingTurret *mine;

				try {
					mine = new BuildingTurret(side, mapPos);
				}
				catch (int) {
					return;
				}

				GameObject *mineObj = new GameObject("Turret");
				Transform* transform = new Transform(screenPos.x, screenPos.y);
				transform->SetScale({ 100, 100 });
				mineObj->AddComponent(transform);

				Sprite *sprite = new Sprite();
				mine->texture = AEGfxTextureLoad("Data\\Assets\\Turret.png");
				SpriteSource* spriteSource = new SpriteSource(1, 1, mine->texture);
				sprite->SetSpriteSource(spriteSource);
				mine->mesh = MeshCreateQuad(0.5, 0.5, 1, 1);
				sprite->SetMesh(mine->mesh);

				mineObj->AddComponent(sprite);
				mineObj->AddComponent(mine);

				Space::GetLayer(0)->GetGameObjectManager()->Add(*mineObj);
			}
		}

		bool buildingMenu = controls.gamepad->GetButtonTriggered(BUILDING_MENU);

		if (buildingMenu || (side == sRight && AEInputCheckTriggered('B'))) {
			if (PopupMenu::Exists(side)) PopupMenu::DestroyMenu(side);
			else PopupMenu::CreateMenu(side, PopupMenu::MenuType::Building);
		}
		if (PopupMenu::Exists(side)) PopupMenu::Update(side, *controls.gamepad, dt, tilemap->GetPosOnMap(cursPos), tilemap->GetPosOnScreen(tilemap->GetPosOnMap(cursPos)));
		else {
			// If the select button is pressed down this frame, set the select rectangle start positions.
			if (controls.gamepad->GetButtonTriggered(SELECT) || AEInputCheckTriggered(VK_RETURN)) { ///TODO: Remove AEInput check
				rectStartPos = cursorNode;

				//Check if we are trying to select a building, and open the menu associated with that building if we are.
				for (GameObject* building : Building::allBuildings) {
					Transform *transform = building->GetComponent<Transform>();
					if (transform && GridManager::GetInstance().ConvertToGridPoint(transform->GetTranslation()) == cursorNode->gridPos()) {
						((Building*)building->GetComponent("Building"))->OpenMenu(tilemap->GetPosOnMap(cursPos), cursPos);
					}
				}
			}
		}

		// If the move button is down, set the path for all selected units.
		if (controls.gamepad->GetButtonTriggered(MOVE)) {
			for (SelectedUnit &unit : selectedUnits)
			{
				unit.unit->SetPath(unit.path);
				unit.unit->SetNextState(BehaviorUnit::cUnitMove);
			}
			selectedUnits.clear();
		}
		if (controls.gamepad->GetButtonReleased(SELECT)) {
			CalculateOffsets();
			rectEndPos = nullptr;
		}

		// Are we selecting units?
		if (controls.gamepad->GetButton(SELECT)) {
			if (cursorNode != rectEndPos) {
				rectEndPos = cursorNode;
				SelectUnits();
			}
		} else {
			if (selectedUnits.size() > 0)
			{
				if (cursorNode != prevTarget) {
					prevTarget = cursorNode;
					for (SelectedUnit& unit : selectedUnits)
					{
						Vector2D pos = GridManager::GetInstance().ConvertToGridPoint(cursor.transform->GetTranslation()) + unit.offset;
						unit.path = Pathfinder::FindPath(unit.unit->GetNode(), GridManager::GetInstance().GetNode((int)pos.x, (int)pos.y));
					}
				}
			}
			else
			{
				if (controls.gamepad->GetButtonTriggered(SPAWNUNIT))
					CreateUnit("Unit1", cursorNode->gridPos());
			}
		}
		break;
	}
}

void BehaviorArmy::OnExit()
{
	switch (GetCurrentState()) {
	case cArmyNormal:
		unitTypes.clear();
		break;
	}
}

void BehaviorArmy::Draw(Camera *cam) const
{
	Transform territoryTransform(0, 0);
	Vector2D flTrs = frontLine.transform->GetTranslation();
	Vector2D flScl = frontLine.transform->GetScale();
	switch (side) {
	case sLeft: {
		Vector2D tmTopLeft = tilemap->GetTilemapScreenTopLeft();
		territoryTransform.SetTranslation({ (flTrs.x - flScl.x / 2) / 2 + tmTopLeft.x / 2, flTrs.y });
		territoryTransform.SetScale({ (flTrs.x - flScl.x / 2) - tmTopLeft.x, flScl.y });
		break;
	}
	case sRight: {
		Vector2D tmBottomRight = tilemap->GetTilemapScreenBottomRight();
		territoryTransform.SetTranslation({ (flTrs.x + flScl.x / 2) / 2 + tmBottomRight.x / 2, flTrs.y });
		territoryTransform.SetScale({ tmBottomRight.x - (flTrs.x + flScl.x / 2), flScl.y });
		break;
	}
	}
	float alpha = path.sprite->GetAlpha();
	path.sprite->SetAlpha(0.25f);
	path.sprite->Draw(cam, territoryTransform);
	path.sprite->SetAlpha(alpha);
	if (selectedUnits.size() > 0)
		DrawPath(cam);
}

void BehaviorArmy::Load(rapidjson::Value & obj)
{
	if (obj.HasMember("Name") && obj["Name"].IsString()) {
		string path = "data\\";
		path += obj["Name"].GetString();
		path += ".ini";

		ifstream workingFile(path);

		string line = "";
		BehaviorUnit::Traits unit;

		while (getline(workingFile, line))
		{
			unit = BehaviorUnit::Traits();

			stringstream reader (line);

			reader >> unit.name;
			reader >> unit.strength;
			reader >> unit.agility;
			reader >> unit.defense;
			reader >> unit.ability;
			reader >> unit.weapon;
			reader >> unit.item1;
			reader >> unit.item2;

			unitTypes.push_back(unit);
		}
	}
	if (obj.HasMember("Side") && obj["Side"].IsString()) {
		if (strcmp(obj["Side"].GetString(), "Left") == 0) {
			side = sLeft;
		} else if (strcmp(obj["Side"].GetString(), "Right") == 0) {
			side = sRight;
		} else {
			side = sIllegal;
		}

		Building::InitializeBuildings(side);
	}
	if (obj.HasMember("FrontLineName") && obj["FrontLineName"].IsString()) {
		frontLine.dispObjName = obj["FrontLineName"].GetString();
	} else {
		frontLine.dispObjName = "";
	}
	if (obj.HasMember("FrontLineStart") && obj["FrontLineStart"].IsInt()) {
		frontLine.start = obj["FrontLineStart"].GetInt();
	} else {
		switch (side) {
		case sLeft:
			frontLine.start = 0;
			break;
		case sRight:
			frontLine.start = -1;
			break;
		}
	}
	frontLine.pos = -1;
	if (obj.HasMember("FundsDisplayName") && obj["FundsDisplayName"].IsString()) {
		funds.dispObjName = obj["FundsDisplayName"].GetString();
	} else {
		funds.dispObjName = "";
	}
	if (obj.HasMember("StartFunds") && obj["StartFunds"].IsUint()) {
		funds.startAmount = obj["StartFunds"].GetUint();
	} else {
		funds.startAmount = 0;
	}
	funds.amount = 0;
	numUnits = 0;
	if (obj.HasMember("PathLineName") && obj["PathLineName"].IsString()) {
		path.lineDispName = obj["PathLineName"].GetString();
	}
	if (obj.HasMember("GamepadNum") && obj["GamepadNum"].IsUint() && obj["GamepadNum"].GetUint() >= 0) {
		controls.gamepad = &Gamepad::GetGamepad(obj["GamepadNum"].GetUint());
	}
	if (obj.HasMember("Controls") && obj["Controls"].IsString()) {
		string path = "Data\\";
		path.append(obj["Controls"].GetString());
		path.append(".json");

		ifstream file(path);
		string line, contents;

		rapidjson::Document levelDoc;

		// Load the file into contents.
		while (getline(file, line))
		{
			contents.append(line);
		}

		levelDoc.Parse(contents.c_str());
		assert(levelDoc.IsObject());
		controls.list.Load(levelDoc);
	}
	if (obj.HasMember("CursorName") && obj["CursorName"].IsString()) {
		cursor.objName = obj["CursorName"].GetString();
	}
}

void BehaviorArmy::CreateUnit(const char *unitName, Vector2D startPos)
{
	BehaviorUnit::Traits unitData = GetUnitData(unitName);
	if (funds.amount < unitData.GetCost()) return;
	if (!LegalSpawn(startPos)) return;
	funds.amount -= unitData.GetCost();
	UpdateFundsText();
	GameObject *go = GetParent()->GetGameObjectManager()->GetArchetype("UnitArchetype");
	if (!go) {
		Trace::GetInstance().GetStream() << "No Unit archetype found" << std::endl;
		return;
	}
	go = new GameObject(*go);
	go->SetName("Unit");

	Transform *t = go->GetComponent<Transform>();
	t->SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(startPos));

	BehaviorUnit *bh = go->GetComponent<BehaviorUnit>();
	if (!bh) {
		Trace::GetInstance().GetStream() << "No \"BehaviorUnit\" Component found on \"UnitArchetype\"" << std::endl;
		delete go;
		return;
	}
	bh->Init(unitData, this);

	GetParent()->GetGameObjectManager()->Add(*go);
}

bool BehaviorArmy::LegalSpawn(Vector2D pos)
{
	vector<GameObject*> unitGOs = GetParent()->GetGameObjectManager()->GetObjectsByName("Unit");
	if (!GridManager::GetInstance().GetNode(pos)->open)
		return false;
	if (pos.x < 0 || pos.x >= tilemap->GetTilemapWidth() || pos.y < 0 || pos.y >= tilemap->GetTilemapHeight())
		return false;
	return BehindFrontLine(pos);
}

bool BehaviorArmy::BehindFrontLine(Vector2D pos)
{
	switch (side) {
	case sLeft:
		return pos.X() <= frontLine.pos;
	case sRight:
		return pos.X() >= frontLine.pos;
	default:
		return false;
	}
}

void BehaviorArmy::SelectUnits()
{
	selectedUnits.clear();

	for (GameObject* unit : BehaviorUnit::allUnits)
	{
		if (unit->GetComponent<BehaviorUnit>()->GetArmy() != this)
			continue;

		SelectedUnit selUnit;
		selUnit.unit = unit->GetComponent<BehaviorUnit>();
		if ((selUnit.unit->GetGridPos().X() <= rectEndPos->x && selUnit.unit->GetGridPos().X() >= rectStartPos->x) ||
			(selUnit.unit->GetGridPos().X() >= rectEndPos->x && selUnit.unit->GetGridPos().X() <= rectStartPos->x))
		{
			if ((selUnit.unit->GetGridPos().Y() <= rectEndPos->y && selUnit.unit->GetGridPos().Y() >= rectStartPos->y) ||
				(selUnit.unit->GetGridPos().Y() >= rectEndPos->y && selUnit.unit->GetGridPos().Y() <= rectStartPos->y))
			{
				selectedUnits.push_back(selUnit);
			}
		}
	}
}

// Function is used to figure out the midpoint of all selected units, then determines the offset for each unit from this point.
// Allows for the offset to be used when determining target position.
void BehaviorArmy::CalculateOffsets()
{
	// Figure out the mean point of all selected units.
	Vector2D midpoint;
	
	for (SelectedUnit unit : selectedUnits)
	{
		midpoint += unit.unit->GetGridPos();
	}

	midpoint /= (float)selectedUnits.size();

	// Use the calculated point to determine offsets for all selected units.
	for (SelectedUnit unit : selectedUnits)
	{
		unit.offset = unit.unit->GetGridPos() - midpoint;
	}
}

void BehaviorArmy::DrawPath(Camera *cam) const
{
	for (SelectedUnit unit : selectedUnits)
	{
		if (unit.path.size() == 0)
			continue;

		Node* lastNode = nullptr;
		
		Vector2D straightScale = Vector2D((float)GridManager::GetInstance().tileWidth, (float)GridManager::GetInstance().tileHeight * 0.5f);
		//Vector2D diagonalScale = Vector2D()

		Transform drawTrans = Transform();
		drawTrans.SetScale({ straightScale.x * 1.25f, straightScale.y * 0.75f });
		
		Transform diamondT = Transform();
		diamondT.SetRotation((float)M_PI / 4);
		diamondT.SetScale({ straightScale.y * 0.75f, straightScale.y * 0.75f });
		diamondT.SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(unit.path[0]->gridPos()));

		for (Node* node : unit.path)
		{
			if (lastNode)
			{
				Vector2D subbed = lastNode->gridPos() - node->gridPos();
				
				float rot = atan2(-subbed.y, subbed.x);

				drawTrans.SetRotation(rot);
				drawTrans.SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(node->gridPos()).Midpoint(GridManager::GetInstance().ConvertToWorldPoint(lastNode->gridPos())));
				
				path.sprite->Draw(cam, drawTrans);

				diamondT.SetRotation(rot + (float)M_PI / 4.f);
			}

			diamondT.SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(node->gridPos()));
			path.sprite->Draw(cam, diamondT);

			diamondT.SetRotation(diamondT.GetRotation() - (float)M_PI / 4.f);
			path.sprite->Draw(cam, diamondT);

			lastNode = node;
		}

		Vector2D unitPos = unit.unit->GetGridPos();
		Vector2D subbed = lastNode->gridPos() - unitPos;

		float rot = atan2(-subbed.y, subbed.x);

		drawTrans.SetRotation(rot);

		drawTrans.SetTranslation(GridManager::GetInstance().ConvertToWorldPoint(unitPos).Midpoint(GridManager::GetInstance().ConvertToWorldPoint(lastNode->gridPos())));

		path.sprite->Draw(cam, drawTrans);
	}
}

BehaviorUnit::Traits BehaviorArmy::GetUnitData(const char * name_) const
{
	for (BehaviorUnit::Traits unit : unitTypes) {
		if (strcmp(name_, unit.name.c_str()) == 0)
			return unit;
	}
	return BehaviorUnit::Traits({ -1 });
}

void BehaviorArmy::UpdateFundsText()
{
	if (funds.text) {
		funds.text->SetText(std::to_string((unsigned)funds.amount).c_str());
	}
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

/*unsigned BehaviorArmy::UnitData::GetCost()
{
	unsigned cost = (unsigned)(((float)hp / 100)*((float)hp / 100)*10 + 0.5f);
	cost += (unsigned)(((float)damage / 100)*((float)damage / 100)*10 + 0.5f);
	cost += (unsigned)(((float)speed / 50)*((float)speed / 50)*10 + 0.5f);
	if (BehaviorArmy::costType == 1) cost += (unsigned)(army->numUnits * 1.5);
	return cost;
}*/
