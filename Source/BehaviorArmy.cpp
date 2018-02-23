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
#include "BehaviorUnit.h"
#include "Transform.h"
#include "Trace.h"
#include "Engine.h"
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
		Behavior("BehaviorArmy"), diamondTransform(0, 0)
{
	diamondTransform.SetRotation((float)M_PI / 4);
	SetCurrentState(cBehaviorInvalid);
	SetNextState(cArmyNormal);
}

void BehaviorArmy::PushFrontLine(Vector2D pos)
{
	vector<GameObject*> unitGOs = GetParent()->GetObjectManager()->GetObjectsByName("Unit");
	vector<GameObject*> armyGOs = GetParent()->GetObjectManager()->GetObjectsByName("Army");
	switch (side) {
	case sLeft:
		if (pos.x > frontLine) {
			frontLine = (int)pos.x;
			for (GameObject *unit : unitGOs) {
				Transform *t = (Transform*)unit->GetComponent("Transform");
				BehaviorUnit *bu = (BehaviorUnit*)unit->GetComponent("BehaviorUnit");
				if (!t || !bu) continue;
				Vector2D mapPos = tilemap->GetPosOnMap(t->GetTranslation());
				if (bu->GetArmy()->GetSide() != side && mapPos.x <= frontLine) {
					frontLine = (int)mapPos.x - 1;
				}
			}
		}
		if (frontLine < 0) frontLine = 0;
		if (frontLine >= tilemap->GetTilemapWidth() - 1) frontLine = tilemap->GetTilemapWidth() - 2;
		if (flTransform)
			flTransform->SetTranslation(tilemap->GetPosOnScreen({ (float)frontLine, (float)(tilemap->GetTilemapHeight()) / 2 }) + 
										Vector2D((float)(tilemap->GetTileWidth())/2, (float)(tilemap->GetTileHeight())/2));
		break;
	case sRight:
		if (pos.x < frontLine) {
			frontLine = (int)pos.x;
			for (GameObject *unit : unitGOs) {
				Transform *t = (Transform*)unit->GetComponent("Transform");
				BehaviorUnit *bu = (BehaviorUnit*)unit->GetComponent("BehaviorUnit");
				if (!t || !bu) continue;
				Vector2D mapPos = tilemap->GetPosOnMap(t->GetTranslation());
				if (bu->GetArmy()->GetSide() != side && mapPos.x >= frontLine) {
					frontLine = (int)mapPos.x + 1;
				}
			}
		}
		if (frontLine >= tilemap->GetTilemapWidth()) frontLine = tilemap->GetTilemapWidth()-1;
		if (frontLine <= 0) frontLine = 1;
		if (flTransform)
			flTransform->SetTranslation(tilemap->GetPosOnScreen({ (float)frontLine, (float)(tilemap->GetTilemapHeight()) / 2 }) + 
										Vector2D(-(float)(tilemap->GetTileWidth())/2, (float)(tilemap->GetTileHeight())/2));
		break;
	}
	for (GameObject *army : armyGOs) {
		BehaviorArmy *ba = (BehaviorArmy*)army->GetComponent("BehaviorArmy");
		if (!ba || ba->frontLine < 0) continue;
		if (ba->GetSide() != side) {
			switch (ba->GetSide()) {
			case sLeft:
				if (pos.x <= ba->frontLine) {
					ba->frontLine = (int)pos.x - 1;
					if (ba->frontLine < 0) ba->frontLine = 0;
				}
				if (ba->flTransform)
					ba->flTransform->SetTranslation(tilemap->GetPosOnScreen({ (float)ba->frontLine, (float)(tilemap->GetTilemapHeight()) / 2 }) + 
												Vector2D((float)(tilemap->GetTileWidth())/2, (float)(tilemap->GetTileHeight())/2));
				break;
			case sRight:
				if (pos.x >= ba->frontLine) {
					ba->frontLine = (int)pos.x + 1;
					if (ba->frontLine >= tilemap->GetTilemapWidth()) ba->frontLine = tilemap->GetTilemapWidth()-1;
				}
				if (ba->flTransform)
					ba->flTransform->SetTranslation(tilemap->GetPosOnScreen({ (float)ba->frontLine, (float)(tilemap->GetTilemapHeight()) / 2 }) + 
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

unsigned BehaviorArmy::GetFunds()
{
	return funds;
}

bool BehaviorArmy::TakeFromFunds(unsigned amount)
{
	if (funds >= amount) {
		funds -= amount;
		UpdateFundsText();
		return true;
	} else {
		return false;
	}
}

void BehaviorArmy::AddToFunds(unsigned amount)
{
	funds += amount;
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
		tilemap = (Tilemap*)GetParent()->GetObjectManager()->GetObjectByName("Tilemap")->GetComponent("Tilemap");
		GameObject *fl = GetParent()->GetObjectManager()->GetObjectByName(flObjName.c_str());
		if (fl) flTransform = (Transform*)fl->GetComponent("Transform");
		else flTransform = nullptr;
		GameObject *fd = GetParent()->GetObjectManager()->GetObjectByName(fundsObjName.c_str());
		if (fd) fundsText = (Text*)fd->GetComponent("Text");
		else fundsText = nullptr;
		//f32 winWidth = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		//f32 winHeight = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		switch (side) {
		case sLeft:
			for (int i = 0; i < tilemap->GetTilemapWidth()-1; i++) path_.push_back({ 1, 0 });
			flStart = flStart < 0 ? 0 : flStart;
			/*camera = Engine::GetInstance().AddCamera({AEGfxGetWinMinX() + winWidth/4, AEGfxGetWinMinY() + winHeight/2}, 
													 {AEGfxGetWinMinX(), AEGfxGetWinMaxY()}, {AEGfxGetWinMaxX(), AEGfxGetWinMinY()}, 
													 {0, 0});*/
			break;
		case sRight:
			for (int i = 0; i < tilemap->GetTilemapWidth()-1; i++) path_.push_back({ -1, 0 });
			flStart = flStart < 0 ? tilemap->GetTilemapWidth() - 1 : flStart;
			/*camera = Engine::GetInstance().AddCamera({AEGfxGetWinMaxX() - winWidth/4, AEGfxGetWinMinY() + winHeight/2}, 
													 {AEGfxGetWinMinX(), AEGfxGetWinMaxY()}, {AEGfxGetWinMaxX(), AEGfxGetWinMinY()}, 
													 {0, 0});*/
			break;
		}
		frontLine = flStart;
		if (flTransform) {
			PushFrontLine({ (float)frontLine, 0 });
		}
		funds = startFunds;
		UpdateFundsText();
		//cursor = Vector2D(0, 0);
		cursor = (Transform*)GetParent()->GetObjectManager()->GetObjectByName(cursorObjName.c_str())->GetComponent("Transform");
		GameObject *pathArchetype = GetParent()->GetObjectManager()->GetArchetype(pathLineName.c_str());
		if (pathArchetype) {
			pathSprite = (Sprite*)pathArchetype->GetComponent("Sprite");
			pathTransform = (Transform*)pathArchetype->GetComponent("Transform");
			if (pathTransform) {
				Vector2D pathScl = pathTransform->GetScale();
				diamondTransform.SetScale({pathScl.y, pathScl.y});
			}
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
		gamepad.Update();
		Vector2D curspos = cursor->GetScreenTranslation();
		curspos.x += gamepad.GetAxis(Gamepad::aLStickX) * 2000 * dt;
		curspos.y += gamepad.GetAxis(Gamepad::aLStickY) * 2000 * dt;
		Trace::GetInstance().GetStream() << gamepad.GetAxisNoDeadzone(Gamepad::aLStickX) << std::endl;
		Trace::GetInstance().GetStream() << gamepad.GetAxisNoDeadzone(Gamepad::aLStickY) << std::endl;
		Vector2D tmTopLeft = tilemap->GetTilemapScreenTopLeft();
		Vector2D tmBottomRight = tilemap->GetTilemapScreenBottomRight();
		if (gamepad.GetButtonTriggered(Gamepad::bDpadUp)) {
			curspos.y += tilemap->GetTileHeight();
			if (curspos.y > tmTopLeft.y)
				curspos.y -= tilemap->GetTileHeight() * tilemap->GetTilemapHeight();
			curspos = tilemap->GetPosOnScreen(tilemap->GetPosOnMap(curspos));
		} else if (gamepad.GetButtonTriggered(Gamepad::bDpadDown)) {
			curspos.y -= tilemap->GetTileHeight();
			if (curspos.y < tmBottomRight.y)
				curspos.y += tilemap->GetTileHeight() * tilemap->GetTilemapHeight();
			curspos = tilemap->GetPosOnScreen(tilemap->GetPosOnMap(curspos));
		} else if (gamepad.GetButtonTriggered(Gamepad::bDpadLeft)) {
			curspos.x -= tilemap->GetTileHeight();
			if (curspos.x < tmTopLeft.x)
				curspos.x += tilemap->GetTileWidth() * tilemap->GetTilemapWidth();
			curspos = tilemap->GetPosOnScreen(tilemap->GetPosOnMap(curspos));
		} else if (gamepad.GetButtonTriggered(Gamepad::bDpadRight)) {
			curspos.x += tilemap->GetTileHeight();
			if (curspos.x > tmBottomRight.x)
				curspos.x -= tilemap->GetTileWidth() * tilemap->GetTilemapWidth();
			curspos = tilemap->GetPosOnScreen(tilemap->GetPosOnMap(curspos));
		}
		if (gamepad.GetAxis(Gamepad::aRTrigger) >= 0.8) {
			if (gamepad.GetAxis(Gamepad::aLTrigger) >= 0.8) {
				if (gamepad.GetAxisLastFrame(Gamepad::aLTrigger) < 0.8)
					curspos = tilemap->GetPosOnScreen(editStartPos);
				if (gamepad.GetButton(Gamepad::bB))
					SelectUnits(curspos, true);
				else SelectUnits(curspos);
			} else {
				if (gamepad.GetAxisLastFrame(Gamepad::aLTrigger) >= 0.8) {
					if (tilemap->GetPosOnMap(curspos) != editLastPos)
						curspos = tilemap->GetPosOnScreen(editLastPos);
					editExtraLastPos = { -1, -1 };
				}
				if (!editUnit) {
					SelectUnits(curspos);
				} else if (editUnit->GetParent()->IsDestroyed()) {
					editUnit = nullptr;
				} else {
					if (curspos.x <= tmTopLeft.x)
						curspos.x = tmTopLeft.x;
					if (curspos.x >= tmBottomRight.x)
						curspos.x = tmBottomRight.x - 1;
					if (curspos.y >= tmTopLeft.y)
						curspos.y = tmTopLeft.y;
					if (curspos.y <= tmBottomRight.y)
						curspos.y = tmBottomRight.y - 1;
					if (tilemap->GetPosOnMap(curspos) != editLastPos) {
						Vector2D delta;
						while (tilemap->GetPosOnMap(curspos) != editLastPos) {
							delta = (tilemap->GetPosOnMap(curspos) - editLastPos).Normalized();
							if (AddToEditPath(delta))
								editLastPos += delta;
							else curspos = tilemap->GetPosOnScreen(editLastPos);
						}
						//AddToEditPath(tilemap->GetPosOnMap(curspos) - editLastPos);
						//editLastPos = tilemap->GetPosOnMap(curspos);
					}
					if (gamepad.GetButtonTriggered(Gamepad::bX)) {
						if (!editPath.empty()) {
							editPath.clear();
							curspos = tilemap->GetPosOnScreen(editStartPos);
							editLastPos = editStartPos;
						}
					}
					if (!editPath.empty() && gamepad.GetButtonTriggered(Gamepad::bB)) {
						Vector2D d = editPath[editPath.size() - 1];
						editPath.pop_back();
						editLastPos = tilemap->GetPosOnMap(curspos) - d;
						curspos = tilemap->GetPosOnScreen(editLastPos);
					}
					if (gamepad.GetButtonTriggered(Gamepad::bY)) {
						for (Vector2D d : path_) {
							if (AddToEditPath(d)) {
								editLastPos = tilemap->GetPosOnMap(curspos) + d;
								curspos = tilemap->GetPosOnScreen(editLastPos);
							}
						}
					}
					/*if (gamepad.GetButtonTriggered(Gamepad::bDpadUp))
						AddToEditPath({ 0, -1 });
					if (gamepad.GetButtonTriggered(Gamepad::bDpadDown))
						AddToEditPath({ 0, 1 });
					if (gamepad.GetButtonTriggered(Gamepad::bDpadLeft))
						AddToEditPath({ -1, 0 });
					if (gamepad.GetButtonTriggered(Gamepad::bDpadRight))
						AddToEditPath({ 1, 0 });*/
				}
			}
		} else {
			if (editUnit) {
				editUnit->AddToPath(editPath);
				editUnit = nullptr;
				for (BehaviorUnit *extraUnit : editExtraUnits)
					extraUnit->AddToPath(editPath);
				if (!editExtraUnits.empty())
					editExtraUnits.clear();
				if (!editExtraStartPos.empty())
					editExtraStartPos.clear();
				if (!editPath.empty())
					editPath.clear();
				editLastPos = { -1, -1 };
				curspos = tilemap->GetPosOnScreen(editStartPos);
			}

			vector<Vector2D> path;// = gamepad.GetButton(Gamepad::bRShoulder) ? path_ : vector<Vector2D>();

			if (gamepad.GetButtonTriggered(Gamepad::bA))
				CreateUnit("Unit1", tilemap->GetPosOnMap(curspos), path);
			else if (gamepad.GetButtonTriggered(Gamepad::bX))
				CreateUnit("Unit2", tilemap->GetPosOnMap(curspos), path);
			else if (gamepad.GetButtonTriggered(Gamepad::bY))
				CreateUnit("Unit3", tilemap->GetPosOnMap(curspos), path);
			else if (gamepad.GetButtonTriggered(Gamepad::bB))
				CreateUnit("Unit4", tilemap->GetPosOnMap(curspos), path);
		}
		Vector2D cursscl = cursor->GetScreenScale();
		if (curspos.x + cursscl.x / 2 > AEGfxGetWinMaxX())
			curspos.x = AEGfxGetWinMaxX() - cursscl.x / 2;
		if (curspos.x - cursscl.x / 2 < AEGfxGetWinMinX())
			curspos.x = AEGfxGetWinMinX() + cursscl.x / 2;
		if (curspos.y + cursscl.y / 2 > AEGfxGetWinMaxY())
			curspos.y = AEGfxGetWinMaxY() - cursscl.y / 2;
		if (curspos.y - cursscl.y / 2 < AEGfxGetWinMinY())
			curspos.y = AEGfxGetWinMinY() + cursscl.y / 2;
		cursor->SetScreenTranslation(curspos);
		//if (camera)
		//	*camera = -curspos;
		//for (unsigned i = 0; i < controls.size(); i++) {
		//	if (controls[i].CheckTriggered()) {
		//		switch (side) {
		//		case sLeft:
		//			CreateUnit("Unit1", { 0, (float)i }, path_);
		//			break;
		//		case sRight:
		//			CreateUnit("Unit1", { (float)tilemap->GetTilemapWidth()-1, (float)i }, path_);
		//			break;
		//		}
		//	}
		//}
		break;
	}
}

void BehaviorArmy::OnExit()
{
	switch (GetCurrentState()) {
	case cArmyNormal:
		units.clear();
		path_.clear();
		break;
	}
}

void BehaviorArmy::Draw() const
{
	if (editUnit)
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
			units.push_back(unit);
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
	}
	if (obj.HasMember("FrontLineName") && obj["FrontLineName"].IsString()) {
		flObjName = obj["FrontLineName"].GetString();
	} else {
		flObjName = "";
	}
	if (obj.HasMember("FrontLineStart") && obj["FrontLineStart"].IsInt()) {
		flStart = obj["FrontLineStart"].GetInt();
	} else {
		switch (side) {
		case sLeft:
			flStart = 0;
			break;
		case sRight:
			flStart = -1;
			break;
		}
	}
	frontLine = -1;
	if (obj.HasMember("FundsDisplayName") && obj["FundsDisplayName"].IsString()) {
		fundsObjName = obj["FundsDisplayName"].GetString();
	} else {
		fundsObjName = "";
	}
	if (obj.HasMember("StartFunds") && obj["StartFunds"].IsUint()) {
		startFunds = obj["StartFunds"].GetUint();
	} else {
		startFunds = 0;
	}
	funds = 0;
	numUnits = 0;
	if (obj.HasMember("PathLineName") && obj["PathLineName"].IsString()) {
		pathLineName = obj["PathLineName"].GetString();
	}
	if (obj.HasMember("GamepadNum") && obj["GamepadNum"].IsInt()) {
		gamepad = Gamepad(obj["GamepadNum"].GetInt());
	}
	if (obj.HasMember("CursorName") && obj["CursorName"].IsString()) {
		cursorObjName = obj["CursorName"].GetString();
	}
}

void BehaviorArmy::CreateUnit(const char *unitName, Vector2D startPos, vector<Vector2D> path)
{
	UnitData unitData = GetUnitData(unitName);
	if (funds < unitData.GetCost()) return;
	if (!LegalSpawn(startPos)) return;
	funds -= unitData.GetCost();
	UpdateFundsText();
	GameObject *go = GetParent()->GetObjectManager()->GetArchetype("UnitArchetype");
	if (!go) {
		Trace::GetInstance().GetStream() << "No Unit archetype found" << std::endl;
		return;
	}
	go = new GameObject(*go);
	go->SetName("Unit");

	Transform *t = (Transform*)go->GetComponent("Transform");
	t->SetTranslation(tilemap->GetPosOnScreen(startPos));

	BehaviorUnit *bh = (BehaviorUnit*)go->GetComponent("BehaviorUnit");
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
	vector<GameObject*> unitGOs = GetParent()->GetObjectManager()->GetObjectsByName("Unit");
	for (GameObject *unit : unitGOs) {
		BehaviorUnit *bu = (BehaviorUnit*)(unit->GetComponent("BehaviorUnit"));
		Transform *t = (Transform*)unit->GetComponent("Transform");
		if (t) {
			if (pos == tilemap->GetPosOnMap(t->GetTranslation()))
				return false;
		}
		if (bu) {
			if (pos == bu->GetNextPos() && bu->IsMoving())
				return false;
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
		return pos.X() <= frontLine;
	case sRight:
		return pos.X() >= frontLine;
	default:
		return false;
	}
}

void BehaviorArmy::SelectUnits(Vector2D &curspos, bool deselect)
{
	if (editUnit) {
		if (!deselect && tilemap->GetPosOnMap(curspos) == editExtraLastPos)
			return;
	} else {
		if (tilemap->GetPosOnMap(curspos) == editLastPos)
			return;
	}
	vector<GameObject*> found = GetParent()->GetObjectManager()->GetObjectsWithFilter([&](GameObject *obj) {
		BehaviorUnit *bu = (BehaviorUnit*)obj->GetComponent("BehaviorUnit");
		if (bu && bu->GetMapPos() == tilemap->GetPosOnMap(curspos))
			return true;
		else return false;
	});
	if (found.size() > 0) {
		if (deselect) {
			BehaviorUnit *unit = (BehaviorUnit*)found[0]->GetComponent("BehaviorUnit");
			if (unit == editUnit && editExtraUnits.size() > 0) {
				editUnit = editExtraUnits[0];
				editStartPos = editExtraStartPos[0];
				editLastPos = editStartPos;
				for (Vector2D d : editPath)
					editLastPos += d;
				editExtraUnits.erase(editExtraUnits.begin());
				editExtraStartPos.erase(editExtraStartPos.begin());
			} else {
				for (unsigned i = 0; i < editExtraUnits.size(); i++) {
					if (unit == editExtraUnits[i]) {
						editExtraUnits.erase(editExtraUnits.begin() + i);
						editExtraStartPos.erase(editExtraStartPos.begin() + i);
						break;
					}
				}
			}
		} else {
			if (!editUnit) {
				editUnit = (BehaviorUnit*)found[0]->GetComponent("BehaviorUnit");
				editStartPos = editUnit->GetMapPos();//tilemap->GetPosOnMap(((Transform*)found[0]->GetComponent("Transform"))->GetTranslation());
				editPath = editUnit->GetPath();
				editLastPos = tilemap->GetPosOnMap(curspos);
				editExtraLastPos = editLastPos;
				if (editUnit->IsMoving()) {
					editStartPos = editUnit->GetNextPos();
					if (editUnit->GetNextPos() != editLastPos)
						editLastPos += editPath[0];
					editPath.erase(editPath.begin());
				}
				for (Vector2D d : editPath)
					editLastPos += d;
				editUnit->ClearPath();
				if (tilemap->GetPosOnMap(curspos) != editLastPos)
					curspos = tilemap->GetPosOnScreen(editLastPos);
			} else {
				BehaviorUnit *unit = (BehaviorUnit*)found[0]->GetComponent("BehaviorUnit");
				if (unit != editUnit) {
					Vector2D startPos = editUnit->IsMoving() ? unit->GetNextPos() : unit->GetMapPos();
					editExtraUnits.push_back(unit);
					unit->ClearPath();
					editExtraStartPos.push_back(startPos);
					editExtraLastPos = tilemap->GetPosOnMap(curspos);
				}
			}
		}
	}
}

bool BehaviorArmy::AddToEditPath(Vector2D dir)
{
	if (!editUnit) return false;
	Vector2D pos = editStartPos;
	for (unsigned i = 0; i <= editExtraStartPos.size(); i++) {
		for (Vector2D d : editPath)
			pos += d;
		pos += dir;
		if (!tilemap->IsMapPosOnMap(pos))
			return false;
		if (i < editExtraStartPos.size())
			pos = editExtraStartPos[i];
	}
	/*if (tilemap->IsMapPosOnMap(pos))
		editPath.push_back(dir);
	else return false;*/
	editPath.push_back(dir);
	return true;
}

void BehaviorArmy::DrawPath() const
{
	if (!pathTransform || !pathSprite) return;
	Vector2D startPos = editStartPos;
	for (unsigned i = 0; i <= editExtraStartPos.size(); i++) {
		Vector2D pos = tilemap->GetPosOnScreen(startPos);
		Vector2D invY = { 1, -1 };
		Transform diamondT = diamondTransform;
		diamondT.SetTranslation(pos);
		pathSprite->SetAlpha(1.0f);
		pathSprite->Draw(diamondT);
		for (Vector2D dir : editPath) {
			dir *= invY;
			pos += tilemap->GetTileSize() * dir / 2;
			pathTransform->SetTranslation(pos);
			pathTransform->SetRotation(dir.GetAngleRadians());
			pathSprite->SetAlpha(0.5f);
			pathSprite->Draw();
			pos += tilemap->GetTileSize() * dir / 2;
			diamondT.SetTranslation(pos);
			pathSprite->SetAlpha(1.0f);
			pathSprite->Draw(diamondT);
		}
		if (i < editExtraStartPos.size())
			startPos = editExtraStartPos[i];
	}
}

BehaviorArmy::UnitData BehaviorArmy::GetUnitData(const char * name_) const
{
	for (UnitData unit : units) {
		if (strcmp(name_, unit.name) == 0)
			return unit;
	}
	return UnitData({ -1 });
}

void BehaviorArmy::UpdateFundsText()
{
	if (fundsText) {
		fundsText->SetText(std::to_string(funds).c_str());
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
	unsigned cost = (unsigned)((float)hp / 10 + 0.5f);
	cost += (unsigned)((float)damage / 10 + 0.5f);
	cost += (unsigned)((float)speed / 5 + 0.5f);
	if (BehaviorArmy::costType == 1) cost += (unsigned)(army->numUnits * 1.5);
	return cost;
}
