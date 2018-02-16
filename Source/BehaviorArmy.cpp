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
	vector<GameObject*> unitGOs = GameObjectManager::GetInstance().GetObjectsByName("Unit");
	switch (side) {
	case sLeft:
		if (pos.x > frontLine) {
			frontLine = (int)pos.x;
			for (GameObject *unit : unitGOs) {
				Transform *t = (Transform*)unit->GetComponent("Transform");
				BehaviorUnit *bu = (BehaviorUnit*)unit->GetComponent("BehaviorUnit");
				if (!t || !bu) return;
				if (bu->GetArmy()->GetSide() != side && t->GetTranslation().x <= frontLine) {
					frontLine = (int)t->GetTranslation().x - 1;
					if (frontLine < 0) frontLine = 0;
				}
			}
		}
		break;
	case sRight:
		if (pos.x < frontLine) {
			frontLine = (int)pos.x;
			for (GameObject *unit : unitGOs) {
				Transform *t = (Transform*)unit->GetComponent("Transform");
				BehaviorUnit *bu = (BehaviorUnit*)unit->GetComponent("BehaviorUnit");
				if (!t || !bu) return;
				if (bu->GetArmy()->GetSide() != side && t->GetTranslation().x >= frontLine) {
					frontLine = (int)t->GetTranslation().x + 1;
					if (frontLine >= tilemap->GetTilemapWidth()) frontLine = tilemap->GetTilemapWidth();
				}
			}
		}
		break;
	}
}

BehaviorArmy::Side BehaviorArmy::GetSide()
{
	return side;
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
		tilemap = (Tilemap*)GameObjectManager::GetInstance().GetObjectByName("Tilemap")->GetComponent("Tilemap");
		switch (side) {
		case sLeft:
			for (int i = 0; i < tilemap->GetTilemapWidth()-1; i++) path_.push_back({ 1, 0 });
			controls.push_back({ Control::tKeyboard, '1' });
			controls.push_back({ Control::tKeyboard, 'Q' });
			controls.push_back({ Control::tKeyboard, 'A' });
			controls.push_back({ Control::tKeyboard, 'Z' });
			break;
		case sRight:
			for (int i = 0; i < tilemap->GetTilemapWidth()-1; i++) path_.push_back({ -1, 0 });
			controls.push_back({ Control::tKeyboard, '7' });
			controls.push_back({ Control::tKeyboard, 'U' });
			controls.push_back({ Control::tKeyboard, 'J' });
			controls.push_back({ Control::tKeyboard, 'M' });
			break;
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
		for (unsigned i = 0; i < controls.size(); i++) {
			if (controls[i].CheckTriggered()) {
				switch (side) {
				case sLeft:
					CreateUnit("Unit1", { 0, (float)i }, path_);
					break;
				case sRight:
					CreateUnit("Unit1", { (float)tilemap->GetTilemapWidth()-1, (float)i }, path_);
					break;
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
		units.clear();
		controls.clear();
		path_.clear();
		break;
	}
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
	if (obj.HasMember("FrontLine") && obj["FrontLine"].IsInt()) {
		frontLine = obj["FrontLine"].GetInt();
	}
}

void BehaviorArmy::CreateUnit(const char *unitName, Vector2D startPos, vector<Vector2D> path)
{
	if (!LegalSpawn(startPos)) return;
	GameObject *go = GameObjectManager::GetInstance().GetArchetype("UnitArchetype");
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
	UnitData unitData = GetUnitData(unitName);
	if (unitData.hp == -1) {
		Trace::GetInstance().GetStream() << "No Unit \"" << unitName << "\" found" << std::endl;
		delete go;
		return;
	}
	bh->Init(unitData, path, tilemap);

	GameObjectManager::GetInstance().Add(*go);
}

bool BehaviorArmy::LegalSpawn(Vector2D pos)
{
	vector<GameObject*> unitGOs = GameObjectManager::GetInstance().GetObjectsByName("Unit");
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

BehaviorArmy::UnitData BehaviorArmy::GetUnitData(const char * name_) const
{
	for (UnitData unit : units) {
		if (strcmp(name_, unit.name) == 0)
			return unit;
	}
	return UnitData({ -1 });
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
