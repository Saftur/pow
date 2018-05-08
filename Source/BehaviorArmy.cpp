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
#include "GameObjectManager.h"
#include "Cursor.h"
#include "BehaviorUnit.h"
#include "Transform.h"
#include "Trace.h"
#include "Engine.h"
#include "PopupMenu.h"
#include "Building.h"
#include "Controls.h"
#include <iostream>
#include <sstream>
using std::ifstream;
using std::stringstream;

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
	vector<GameObject*> unitGOs = GetParent()->GetObjectManager()->GetObjectsByName("Unit");
	vector<GameObject*> armyGOs = GetParent()->GetObjectManager()->GetObjectsByName("Army");
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
		tilemap = GetParent()->GetObjectManager()->GetObjectByName("Tilemap")->GetComponent<Tilemap>();
		GameObject *fl = GetParent()->GetObjectManager()->GetObjectByName(frontLine.dispObjName.c_str());
		if (fl) frontLine.transform = fl->GetComponent<Transform>();
		else frontLine.transform = nullptr;
		GameObject *fd = GetParent()->GetObjectManager()->GetObjectByName(funds.dispObjName.c_str());
		if (fd) funds.text = fd->GetComponent<Text>();
		else funds.text = nullptr;
		funds.amount = (float)funds.startAmount;
		UpdateFundsText();
		GameObject *cursorObj = GetParent()->GetObjectManager()->GetObjectByName(cursor.objName.c_str());
		cursor.transform = cursorObj->GetComponent<Transform>();
		cursorObj->GetComponent<Cursor>()->SetGamepad(controls.gamepad);
		cursor.sprite = cursorObj->GetComponent<Sprite>();
		GameObject *pathArchetype = GetParent()->GetObjectManager()->GetArchetype(path.lineDispName.c_str());
		if (pathArchetype) {
			path.sprite = pathArchetype->GetComponent<Sprite>();
			path.transform = pathArchetype->GetComponent<Transform>();
			/*if (path.transform) {
				Vector2D pathScl = path.transform->GetScale();
			}*/
		}
		switch (side) {
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
		Vector2D curspos = cursor.transform->GetScreenTranslation();
		// Tilemap top left point
		Vector2D tmTopLeft = tilemap->GetTilemapScreenTopLeft();
		// Tilemap bottom right point
		Vector2D tmBottomRight = tilemap->GetTilemapScreenBottomRight();
		// Cursor Node position
		Node cursorNode = Grid::GetInstance().ConvertToGridPoint(curspos);

		// If any extra selected units are destroyed, deselect them
		for (unsigned i = 0; i < selectedUnits.size(); i++) {
			if (selectedUnits[i].unit->GetParent()->IsDestroyed()) {
				selectedUnits.erase(selectedUnits.begin() + i);
				i--;
			}
		}

		bool buildingMenu = controls.gamepad->GetButtonTriggered(BUILDING_MENU);

		if (buildingMenu && !PopupMenu::Exists(side)) PopupMenu::CreateMenu(side, PopupMenu::MenuType::Building,
			tilemap->GetPosOnMap(curspos), tilemap->GetPosOnScreen(tilemap->GetPosOnMap(curspos)));
		else if (buildingMenu && PopupMenu::Exists(side)) PopupMenu::DestroyMenu(side);

		if (PopupMenu::Exists(side)) PopupMenu::Update(side, *controls.gamepad, controls.list, dt);
		else {
			if (controls.gamepad->GetButtonTriggered(SELECT)) {
				rectStartPos = cursorNode;
			}
			if (controls.gamepad->GetButtonTriggered(MOVE)) {
				for (SelectedUnit &unit : selectedUnits) {
					unit.unit->SetPath(unit.path);
					unit.unit->SetNextState(BehaviorUnit::cUnitMove);
				}
				selectedUnits.clear();
			}
			if (controls.gamepad->GetButtonReleased(SELECT)) {
				CalculateOffsets();
				rectEndPos.gridPos = {-1,-1};
			}

			// Are we selecting units?
			if (controls.gamepad->GetButton(SELECT)) {
				if (cursorNode != rectEndPos) {
					rectEndPos = cursorNode;
					SelectUnits();
				}
			} else {
				if (selectedUnits.size() > 0) {
					if (cursorNode != prevTarget) {
						prevTarget = cursorNode;
						for (SelectedUnit unit : selectedUnits) {
							unit.path = Pathfinding::FindPath(unit.unit->GetNode(), Grid::GetInstance().ConvertToGridPoint(cursor.transform->GetTranslation() + unit.offset));
						}
					}
				} else {
					if (controls.gamepad->GetButtonTriggered(SPAWNUNIT))
						CreateUnit("Unit1", cursorNode);
				}
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

void BehaviorArmy::Draw() const
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
	path.sprite->Draw(territoryTransform);
	path.sprite->SetAlpha(alpha);
	if (selectedUnits.size() > 0)
		DrawPath();
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

void BehaviorArmy::CreateUnit(const char *unitName, Node startPos)
{
	BehaviorUnit::Traits unitData = GetUnitData(unitName);
	if (funds.amount < unitData.GetCost()) return;
	if (!LegalSpawn(startPos)) return;
	funds.amount -= unitData.GetCost();
	UpdateFundsText();
	GameObject *go = GetParent()->GetObjectManager()->GetArchetype("UnitArchetype");
	if (!go) {
		Trace::GetInstance().GetStream() << "No Unit archetype found" << std::endl;
		return;
	}
	go = new GameObject(*go);
	go->SetName("Unit");

	Transform *t = go->GetComponent<Transform>();
	t->SetTranslation(Grid::GetInstance().ConvertToWorldPoint(startPos));

	BehaviorUnit *bh = go->GetComponent<BehaviorUnit>();
	if (!bh) {
		Trace::GetInstance().GetStream() << "No \"BehaviorUnit\" Component found on \"UnitArchetype\"" << std::endl;
		delete go;
		return;
	}
	bh->Init(unitData, this);

	GetParent()->GetObjectManager()->Add(*go);
}

bool BehaviorArmy::LegalSpawn(Vector2D pos)
{
	vector<GameObject*> unitGOs = GetParent()->GetObjectManager()->GetObjectsByName("Unit");
	for (GameObject *unit : unitGOs) {
		BehaviorUnit *bu = unit->GetComponent<BehaviorUnit>();
		Transform *t = unit->GetComponent<Transform>();
		if (t) 
		{
			if (pos == tilemap->GetPosOnMap(t->GetTranslation()))
				return false;
		}
		if (bu) 
		{
			if (pos == bu->GetGridPos()) 
			{
				return false;
			}
		}
	}
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
		SelectedUnit selUnit;
		selUnit.unit = unit->GetComponent<BehaviorUnit>();
		if ((selUnit.unit->GetGridPos().X() <= rectEndPos.X() && selUnit.unit->GetGridPos().X() >= rectStartPos.X()) ||
			(selUnit.unit->GetGridPos().X() >= rectEndPos.X() && selUnit.unit->GetGridPos().X() <= rectStartPos.X()))
		{
			if ((selUnit.unit->GetGridPos().Y() <= rectEndPos.Y() && selUnit.unit->GetGridPos().Y() >= rectStartPos.Y()) ||
				(selUnit.unit->GetGridPos().Y() >= rectEndPos.Y() && selUnit.unit->GetGridPos().Y() <= rectStartPos.Y()))
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
		midpoint.x += unit.unit->GetParent()->GetComponent<Transform>()->GetTranslation().x;
		midpoint.y += unit.unit->GetParent()->GetComponent<Transform>()->GetTranslation().y;
	}

	midpoint /= (float)selectedUnits.size();

	// Use the calculated point to determine offsets for all selected units.
	for (SelectedUnit unit : selectedUnits)
	{
		unit.offset = unit.unit->GetParent()->GetComponent<Transform>()->GetTranslation() - midpoint;
	}
}

void BehaviorArmy::DrawPath() const
{
	for (SelectedUnit unit : selectedUnits)
	{
		if (!path.transform || !path.sprite) continue;

		Transform targetT = Transform();
		targetT.SetScale(cursor.transform->GetScale() * 0.75);

		targetT.SetTranslation(Grid::GetInstance().ConvertToWorldPoint(unit.unit->GetGridPos()));
		cursor.sprite->Draw(targetT);

		if (unit.path.size() == 0) continue;
		Vector2D startPos = unit.path[0];

		Vector2D pos = tilemap->GetPosOnScreen(startPos);
		Vector2D invY = { 1, -1 };

		Transform diamondT = Transform();
		diamondT.SetRotation((float)M_PI / 4);
		Vector2D pathScl = path.transform->GetScale();
		diamondT.SetScale({ pathScl.y, pathScl.y });
		diamondT.SetTranslation(pos);
		path.sprite->SetAlpha(1.0f);
		path.sprite->Draw(diamondT);

		for (Node node : unit.path)
		{
			node.gridPos *= invY;
			pos += tilemap->GetTileSize() * node.gridPos / 2;
			path.transform->SetTranslation(pos);
			path.transform->SetRotation(node.gridPos.GetAngleRadians());
			path.sprite->SetAlpha(0.5f);
			path.sprite->Draw();
			pos += tilemap->GetTileSize() * node.gridPos / 2;
			diamondT.SetTranslation(pos);
			path.sprite->SetAlpha(1.0f);
			path.sprite->Draw(diamondT);
		}
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
