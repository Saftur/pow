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
#include <iostream>
#include <sstream>
#include "Building.h"
#include "PopupMenu.h"
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
	path.diamondTransform.SetRotation((float)M_PI / 4);
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
		//f32 winWidth = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		//f32 winHeight = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		funds.amount = funds.startAmount;
		UpdateFundsText();
		//cursor = Vector2D(0, 0);
		GameObject *cursorObj = GetParent()->GetObjectManager()->GetObjectByName(cursor.objName.c_str());
		cursor.transform = cursorObj->GetComponent<Transform>();
		cursorObj->GetComponent<Cursor>()->SetGamepad(controls.gamepad);
		cursor.sprite = cursorObj->GetComponent<Sprite>();
		GameObject *pathArchetype = GetParent()->GetObjectManager()->GetArchetype(path.lineDispName.c_str());
		if (pathArchetype) {
			path.sprite = pathArchetype->GetComponent<Sprite>();
			path.transform = pathArchetype->GetComponent<Transform>();
			if (path.transform) {
				Vector2D pathScl = path.transform->GetScale();
				path.diamondTransform.SetScale({pathScl.y, pathScl.y});
				path.targetTransform.SetScale(cursor.transform->GetScale() * 0.75);
			}
		}
		switch (side) {
		case sLeft:
			frontLine.start = frontLine.start < 0 ? 0 : frontLine.start;
			cursor.transform->SetTranslation(tilemap->GetPosOnScreen({ 0, 0 }));
			/*camera = Engine::GetInstance().AddCamera({AEGfxGetWinMinX() + winWidth/4, AEGfxGetWinMinY() + winHeight/2}, 
													 {AEGfxGetWinMinX(), AEGfxGetWinMaxY()}, {AEGfxGetWinMaxX(), AEGfxGetWinMinY()}, 
													 {0, 0});*/
			break;
		case sRight:
			frontLine.start = frontLine.start < 0 ? tilemap->GetTilemapWidth() - 1 : frontLine.start;
			cursor.transform->SetTranslation(tilemap->GetPosOnScreen({ (float)tilemap->GetTilemapWidth() - 1, 0 }));
			/*camera = Engine::GetInstance().AddCamera({AEGfxGetWinMaxX() - winWidth/4, AEGfxGetWinMinY() + winHeight/2}, 
													 {AEGfxGetWinMinX(), AEGfxGetWinMaxY()}, {AEGfxGetWinMaxX(), AEGfxGetWinMinY()}, 
													 {0, 0});*/
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
		// Update the controls.gamepad
		//controls.gamepad->Update();

		// Get current cursor position
		Vector2D curspos = cursor.transform->GetScreenTranslation();

		// Tilemap top left point
		Vector2D tmTopLeft = tilemap->GetTilemapScreenTopLeft();
		// Tilemap bottom right point
		Vector2D tmBottomRight = tilemap->GetTilemapScreenBottomRight();

		// Get current mode
		bool inEditMode = controls.gamepad->GetButton(controls.list["Normal.SwitchCommand"]);
		bool inSelectMode = inEditMode && controls.gamepad->GetButton(controls.list["Command.SwitchSelect"]);

		// If selected unit gets destroyed, deselect it
		if (edit.unit && edit.unit->GetParent()->IsDestroyed()) {
			edit.unit = nullptr;
			edit.pos = { -1, -1 };
			// If several units selected, switch main selection to first one
			if (edit.extra.units.size() > 0) {
				edit.unit = edit.extra.units[0];
				edit.startPos = edit.extra.startPos[0];
				edit.pos = edit.startPos;
				for (Vector2D d : edit.path)
					edit.pos += d;
				edit.extra.units.erase(edit.extra.units.begin());
				edit.extra.startPos.erase(edit.extra.startPos.begin());
				edit.extra.lastPos = tilemap->GetPosOnMap(curspos);
			}
		}

		// If any extra selected units are destroyed, deselect them
		for (unsigned i = 0; i < edit.extra.units.size(); i++) {
			if (edit.extra.units[i]->GetParent()->IsDestroyed()) {
				edit.extra.units.erase(edit.extra.units.begin() + i);
				edit.extra.startPos.erase(edit.extra.startPos.begin() + i);
				i--;
			}
		}

		///TODO: Remove after demo.
		bool createUnitMenu = controls.gamepad->GetButtonTriggered(controls.list["CamUp"]);
		bool researchMenu = controls.gamepad->GetButtonTriggered(controls.list["CamDown"]);
		if (createUnitMenu && !PopupMenu::Exists(side)) PopupMenu::CreateMenu(side, PopupMenu::MenuType::CommandPost, 
			tilemap->GetPosOnMap(curspos), tilemap->GetPosOnScreen(tilemap->GetPosOnMap(curspos)));
		else if (createUnitMenu && PopupMenu::Exists(side)) PopupMenu::DestroyMenu(side); 
		if (researchMenu && !PopupMenu::Exists(side)) PopupMenu::CreateMenu(side, PopupMenu::MenuType::Research, 
			tilemap->GetPosOnMap(curspos), tilemap->GetPosOnScreen(tilemap->GetPosOnMap(curspos)));
		else if (researchMenu && PopupMenu::Exists(side)) PopupMenu::DestroyMenu(side); 


		//Control whether there is a menu open or not.
		bool buildingMenu = controls.gamepad->GetButtonTriggered(controls.list["Normal.BuildingMenu"]);
		if (cursor.objName == "Cursor2") buildingMenu = AEInputCheckTriggered('M'); ///TODO: Remove when controllers are available.

		//If we are hitting the key to open a menu, and one isn't already open, open a building menu.
		if (buildingMenu && !PopupMenu::Exists(side)) PopupMenu::CreateMenu(side, PopupMenu::MenuType::Building, 
			tilemap->GetPosOnMap(curspos), tilemap->GetPosOnScreen(tilemap->GetPosOnMap(curspos)));
		else if (buildingMenu && PopupMenu::Exists(side)) PopupMenu::DestroyMenu(side); //If we hit the building menu button and the menu is open, destroy it.

		if (PopupMenu::Exists(side)) PopupMenu::Update(side, *controls.gamepad, controls.list, dt); //If a menu exists, update it.
		else {
			if (inEditMode) {
				if (inSelectMode) {
					if (controls.gamepad->GetButtonTriggered(controls.list["Command.SwitchSelect"])) {
						if (tilemap->GetPosOnMap(curspos) != edit.startPos)
							curspos = tilemap->GetPosOnScreen(edit.startPos);
						edit.selectMode = edit.smAuto;
					}
					if (controls.gamepad->GetButtonTriggered(controls.list["Select.SwitchManual"])) {
						edit.selectMode = edit.smManual;
					}
					if (controls.gamepad->GetButtonTriggered(controls.list["Select.SwitchAuto"])) {
						edit.selectMode = edit.smAuto;
						edit.extra.lastPos = { -1, -1 };
					}
					if (edit.selectMode == edit.smAuto) {
						if (tilemap->GetPosOnMap(curspos) != edit.extra.lastPos)
							edit.extra.lastPos = { -1, -1 };
						SelectUnits(curspos);
					}
					else if (edit.selectMode == edit.smManual) {
						edit.extra.lastPos = { -1, -1 };
						if (controls.gamepad->GetButtonTriggered(/*Gamepad::bA*/controls.list["Select.Manual.Select"]))
							SelectUnits(curspos);
						if (controls.gamepad->GetButtonTriggered(/*Gamepad::bB*/controls.list["Select.Manual.Deselect"]))
							SelectUnits(curspos, true);
					}
				}
				else {
					if (controls.gamepad->GetButtonReleased(/*Gamepad::bLTrigger*/controls.list["Command.SwitchSelect"])) {
						if (tilemap->GetPosOnMap(curspos) != tilemap->NormalizeMapPos(edit.pos))
							curspos = tilemap->GetPosOnScreen(edit.pos);
						edit.extra.lastPos = { -1, -1 };
					}
					if (!edit.unit) {
						SelectUnits(curspos);
					}
					if (edit.unit) {
						if (curspos.x <= tmTopLeft.x)
							curspos.x = tmTopLeft.x;
						if (curspos.x >= tmBottomRight.x)
							curspos.x = tmBottomRight.x - 1;
						if (curspos.y >= tmTopLeft.y)
							curspos.y = tmTopLeft.y;
						if (curspos.y <= tmBottomRight.y)
							curspos.y = tmBottomRight.y - 1;
						if (tilemap->GetPosOnMap(curspos) != tilemap->NormalizeMapPos(edit.pos)) {
							Vector2D delta;
							while (tilemap->GetPosOnMap(curspos) != tilemap->NormalizeMapPos(edit.pos)) {
								delta = (tilemap->GetPosOnMap(curspos) - edit.pos).Normalized();
								if (delta.x && delta.y) {
									delta.y = 0;
									delta = delta.Normalized();
								}
								if (AddToEditPath(delta))
									edit.pos += delta;
								else curspos = tilemap->GetPosOnScreen(edit.pos);
							}
						}
						if (controls.gamepad->GetButtonTriggered(/*Gamepad::bX*/controls.list["Command.ClearPath"])) {
							if (!edit.path.empty()) {
								edit.path.clear();
								curspos = tilemap->GetPosOnScreen(edit.startPos);
								edit.pos = edit.startPos;
							}
						}
						if (!edit.path.empty() && controls.gamepad->GetButtonTriggered(/*Gamepad::bB*/controls.list["Command.BackPath"])) {
							Vector2D d = edit.path[edit.path.size() - 1];
							edit.path.pop_back();
							edit.pos = tilemap->GetPosOnMap(curspos) - d;
							curspos = tilemap->GetPosOnScreen(edit.pos);
						}
						if (controls.gamepad->GetButtonTriggered(controls.list["Command.Recycle"])) {
							AddToFunds((float)edit.unit->GetRecycleReturns());
							edit.unit->GetParent()->Destroy();
							for (BehaviorUnit *bu : edit.extra.units) {
								AddToFunds((float)bu->GetRecycleReturns());
								bu->GetParent()->Destroy();
							}
							if (!edit.extra.units.empty())
								edit.extra.units.clear();
							if (!edit.extra.startPos.empty())
								edit.extra.startPos.clear();
							edit.pos = { -1, -1 };
						}
					}
				}
			}
			else {
				if (edit.unit) {
					edit.unit->AddToPath(edit.path);
					edit.unit = nullptr;
					for (BehaviorUnit *extraUnit : edit.extra.units)
						extraUnit->AddToPath(edit.path);
					if (!edit.extra.units.empty())
						edit.extra.units.clear();
					if (!edit.extra.startPos.empty())
						edit.extra.startPos.clear();
					if (!edit.path.empty())
						edit.path.clear();
					edit.pos = { -1, -1 };
					curspos = tilemap->GetPosOnScreen(edit.startPos);
				}

				vector<Vector2D> path;// = controls.gamepad->GetButton(Gamepad::bRShoulder) ? path_ : vector<Vector2D>();

				if (controls.gamepad->GetButton(controls.list["Normal.SpawnUnit1"]))
					CreateUnit("Unit1", tilemap->GetPosOnMap(curspos), path);
				else if (controls.gamepad->GetButton(controls.list["Normal.SpawnUnit2"]))
					CreateUnit("Unit2", tilemap->GetPosOnMap(curspos), path);
				else if (controls.gamepad->GetButton(controls.list["Normal.SpawnUnit3"]))
					CreateUnit("Unit3", tilemap->GetPosOnMap(curspos), path);
				else if (controls.gamepad->GetButton(controls.list["Normal.SpawnUnit4"]))
					CreateUnit("Unit4", tilemap->GetPosOnMap(curspos), path);
			}
			break;
		}
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
	if (edit.unit)
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
		UnitData unit;

		while (getline(workingFile, line))
		{
			unit = UnitData();

			stringstream reader (line);

			reader >> unit.name;
			reader >> unit.hp;
			reader >> unit.damage;
			reader >> unit.speed;
			reader >> *((int*)&(unit.ability));

			unit.army = this;
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
		funds.startAmount = obj["StartFunds"].GetFloat();
	} else {
		funds.startAmount = 0;
	}
	funds.amount = 0;
	numUnits = 0;
	if (obj.HasMember("PathLineName") && obj["PathLineName"].IsString()) {
		path.lineDispName = obj["PathLineName"].GetString();
	}
	if (obj.HasMember("GamepadNum") && obj["GamepadNum"].IsUint() && obj["GamepadNum"].GetUint() >= 0) {
		//controlType = ctGamepad;
		controls.gamepad = &Gamepad::GetGamepad(obj["GamepadNum"].GetUint());
	} else {
		//controlType = ctKeyboard;
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

void BehaviorArmy::CreateUnit(const char *unitName, Vector2D startPos, vector<Vector2D> path)
{
	UnitData unitData = GetUnitData(unitName);
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
	t->SetTranslation(tilemap->GetPosOnScreen(startPos));

	BehaviorUnit *bh = go->GetComponent<BehaviorUnit>();
	if (!bh) {
		Trace::GetInstance().GetStream() << "No \"BehaviorUnit\" Component found on \"UnitArchetype\"" << std::endl;
		delete go;
		return;
	}
	if (unitData.hp == -1) {
		Trace::GetInstance().GetStream() << "No Unit \"" << unitName << "\" found" << std::endl;
		delete go;
		return;
	}
	bh->Init(unitData, path, tilemap);

	GetParent()->GetObjectManager()->Add(*go);
}

bool BehaviorArmy::LegalSpawn(Vector2D pos)
{
	vector<GameObject*> unitGOs = GetParent()->GetObjectManager()->GetObjectsWithFilter([&](GameObject* obj) {
		if (obj->GetComponent("BehaviorUnit") || obj->GetComponent("Building"))
			return true;
		return false;
	});
	for (GameObject *unit : unitGOs) {
		BehaviorUnit *bu = unit->GetComponent<BehaviorUnit>();
		Transform *t = unit->GetComponent<Transform>();
		if (t) {
			if (pos == tilemap->GetPosOnMap(t->GetTranslation()))
				return false;
		}
		if (bu) {
			//if (pos == bu->GetNextPos() && bu->IsMoving())
			//	return false;
			if (pos == bu->GetNextPos()) {
				if (bu->IsMoving())
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

void BehaviorArmy::SelectUnits(Vector2D &curspos, bool deselect)
{
	// If we have a unit selected
	if (edit.unit) {
		// and we actually want to select and the cursor is 
		if (!deselect && tilemap->GetPosOnMap(curspos) == edit.extra.lastPos)
			return;
	} else { // If we don't have a unit selected
		// If we want to deselect or the current cursor position is the same as the last editing position
		if (deselect || tilemap->GetPosOnMap(curspos) == edit.pos)
			return;
	}
	vector<GameObject*> found = GetParent()->GetObjectManager()->GetObjectsWithFilter([&](GameObject *obj) {
		BehaviorUnit *bu = obj->GetComponent<BehaviorUnit>();
		if (!obj->IsDestroyed() && bu && bu->GetMapPos() == tilemap->GetPosOnMap(curspos) && bu->GetArmy() == this)
			return true;
		else return false;
	});
	if (found.size() > 0) {
		if (deselect) {
			BehaviorUnit *unit = found[0]->GetComponent<BehaviorUnit>();
			if (unit == edit.unit && edit.extra.units.size() > 0) {
				edit.unit = edit.extra.units[0];
				edit.startPos = edit.extra.startPos[0];
				edit.pos = edit.startPos;
				for (Vector2D d : edit.path)
					edit.pos += d;
				edit.extra.units.erase(edit.extra.units.begin());
				edit.extra.startPos.erase(edit.extra.startPos.begin());
				edit.extra.lastPos = tilemap->GetPosOnMap(curspos);
			} else {
				for (unsigned i = 0; i < edit.extra.units.size(); i++) {
					if (unit == edit.extra.units[i]) {
						edit.extra.units.erase(edit.extra.units.begin() + i);
						edit.extra.startPos.erase(edit.extra.startPos.begin() + i);
						edit.extra.lastPos = tilemap->GetPosOnMap(curspos);
						break;
					}
				}
			}
		} else {
			if (!edit.unit) {
				edit.unit = found[0]->GetComponent<BehaviorUnit>();
				edit.startPos = edit.unit->GetMapPos();//tilemap->GetPosOnMap(((Transform*)found[0]->GetComponent("Transform"))->GetTranslation());
				edit.path = edit.unit->GetPath();
				edit.pos = tilemap->GetPosOnMap(curspos);
				edit.extra.lastPos = edit.pos;
				if (edit.unit->IsMoving()) {
					edit.startPos = edit.unit->GetNextPos();
					if (edit.unit->GetNextPos() != edit.pos)
						edit.pos += edit.path[0];
					edit.path.erase(edit.path.begin());
				}
				for (Vector2D d : edit.path)
					edit.pos += d;
				edit.unit->ClearPath();
				if (tilemap->GetPosOnMap(curspos) != edit.pos)
					curspos = tilemap->GetPosOnScreen(edit.pos);
			} else {
				BehaviorUnit *unit = found[0]->GetComponent<BehaviorUnit>();
				if (unit == edit.unit) return;
				for (BehaviorUnit *u : edit.extra.units)
					if (unit == u) return;
				Vector2D startPos = unit->IsMoving() ? unit->GetNextPos() : unit->GetMapPos();
				Vector2D pos = startPos;
				for (unsigned i = 0; i < edit.path.size(); i++) {
					pos += edit.path[i];
					if (!tilemap->IsMapPosOnMap(pos))
						while (i < edit.path.size()) {
							edit.pos -= edit.path[edit.path.size() - 1];
							edit.path.pop_back();
						}
				}
				edit.extra.units.push_back(unit);
				unit->ClearPath();
				edit.extra.startPos.push_back(startPos);
				edit.extra.lastPos = tilemap->GetPosOnMap(curspos);
			}
		}
	}
}

bool BehaviorArmy::AddToEditPath(Vector2D dir)
{
	if (!edit.unit) return false;
	Vector2D pos = edit.startPos;
	for (unsigned i = 0; i <= edit.extra.startPos.size(); i++) {
		for (Vector2D d : edit.path)
			pos += d;
		pos += dir;
		if (!tilemap->IsMapPosOnMap(pos))
			return false;
		if (i < edit.extra.startPos.size())
			pos = edit.extra.startPos[i];
	}
	/*if (tilemap->IsMapPosOnMap(pos))
		edit.path.push_back(dir);
	else return false;*/
	edit.path.push_back(dir);
	return true;
}

void BehaviorArmy::DrawPath() const
{
	if (!path.transform || !path.sprite) return;
	Vector2D startPos = edit.startPos;
	for (unsigned i = 0; i <= edit.extra.startPos.size(); i++) {
		Vector2D pos = tilemap->GetPosOnScreen(startPos);
		Vector2D invY = { 1, -1 };
		Transform diamondT = path.diamondTransform;
		Transform targetT = path.targetTransform;
		targetT.SetTranslation(pos);
		cursor.sprite->Draw(targetT);
		diamondT.SetTranslation(pos);
		path.sprite->SetAlpha(1.0f);
		path.sprite->Draw(diamondT);
		for (Vector2D dir : edit.path) {
			dir *= invY;
			pos += tilemap->GetTileSize() * dir / 2;
			path.transform->SetTranslation(pos);
			path.transform->SetRotation(dir.GetAngleRadians());
			path.sprite->SetAlpha(0.5f);
			path.sprite->Draw();
			pos += tilemap->GetTileSize() * dir / 2;
			diamondT.SetTranslation(pos);
			path.sprite->SetAlpha(1.0f);
			path.sprite->Draw(diamondT);
		}
		if (i < edit.extra.startPos.size())
			startPos = edit.extra.startPos[i];
	}
}

BehaviorArmy::UnitData BehaviorArmy::GetUnitData(const char * name_) const
{
	for (UnitData unit : unitTypes) {
		if (strcmp(name_, unit.name) == 0)
			return unit;
	}
	return UnitData({ -1 });
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

unsigned BehaviorArmy::UnitData::GetCost()
{
	unsigned cost = (unsigned)(((float)hp / 100)*((float)hp / 100)*10 + 0.5f);
	cost += (unsigned)(((float)damage / 100)*((float)damage / 100)*10 + 0.5f);
	cost += (unsigned)(((float)speed / 50)*((float)speed / 50)*10 + 0.5f);
	if (BehaviorArmy::costType == 1) cost += (unsigned)(army->numUnits * 1.5);
	return cost;
}

/*void BehaviorArmy::ControlOptions::Load(rapidjson::Value & obj)
{
	if (obj.HasMember("Camera")) {
		rapidjson::Value &tmp = obj["Camera"];
		if (obj.HasMember("CameraAxisX") && obj["CameraAxisX"].IsString()) {
			Camera.cameraAxisX = GetAxis(obj["CameraAxisX"].GetString());
		}
		if (obj.HasMember("CameraAxisY") && obj["CameraAxisY"].IsString()) {
			Camera.cameraAxisY = GetAxis(obj["CameraAxisY"].GetString());
		}
		if (obj.HasMember("CameraUp") && obj["CameraUp"].IsString()) {
			Camera.cameraUp = GetButton(obj["CameraUp"].GetString());
		}
		if (obj.HasMember("CameraDown") && obj["CameraDown"].IsString()) {
			Camera.cameraDown = GetButton(obj["CameraDown"].GetString());
		}
		if (obj.HasMember("CameraLeft") && obj["CameraLeft"].IsString()) {
			Camera.cameraLeft = GetButton(obj["CameraLeft"].GetString());
		}
		if (obj.HasMember("CameraUp") && obj["CameraUp"].IsString()) {
			Camera.cameraRight = GetButton(obj["CameraRight"].GetString());
		}
	}
}

int BehaviorArmy::ControlOptions::GetAxis(const char * name)
{
	if (strcmp(name, "LStickX") == 0)
		return Gamepad::aLStickX;
	if (strcmp(name, "LStickY") == 0)
		return Gamepad::aLStickY;
	if (strcmp(name, "RStickX") == 0)
		return Gamepad::aRStickX;
	if (strcmp(name, "RStickY") == 0)
		return Gamepad::aRStickY;
	if (strcmp(name, "LTrigger") == 0)
		return Gamepad::aLTrigger;
	if (strcmp(name, "RTrigger") == 0)
		return Gamepad::aRTrigger;
	return Gamepad::aUnbound;
}

int BehaviorArmy::ControlOptions::GetButton(const char * name)
{
	if (strcmp(name, "A") == 0)
		return Gamepad::bA;
	if (strcmp(name, "A") == 0)
		return Gamepad::bA;
	if (strcmp(name, "A") == 0)
		return Gamepad::bA;
	if (strcmp(name, "A") == 0)
		return Gamepad::bA;
	if (strcmp(name, "DpadUp") == 0)
		return Gamepad::bDpadUp;
	if (strcmp(name, "DpadDown") == 0)
		return Gamepad::bDpadDown;
	if (strcmp(name, "DpadLeft") == 0)
		return Gamepad::bDpadLeft;
	if (strcmp(name, "DpadRight") == 0)
		return Gamepad::bDpadRight;
	if (strcmp(name, "LShoulder") == 0)
		return Gamepad::bLShoulder;
	if (strcmp(name, "RShoulder") == 0)
		return Gamepad::bRShoulder;
	if (strcmp(name, "LThumb") == 0)
		return Gamepad::bLThumb;
	if (strcmp(name, "RThumb") == 0)
		return Gamepad::bRThumb;
	if (strcmp(name, "Back") == 0)
		return Gamepad::bBack;
	if (strcmp(name, "Start") == 0)
		return Gamepad::bStart;
	if (strcmp(name, "LTrigger") == 0)
		return Gamepad::bLTrigger;
	if (strcmp(name, "RTrigger") == 0)
		return Gamepad::bRTrigger;
	return Gamepad::bUnbound;
}*/
