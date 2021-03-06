//------------------------------------------------------------------------------
//
// File Name:	BehaviorArmy.cpp
// Author(s):	Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
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
#include "Transform.h"
#include "Trace.h"
#include "Engine.h"
#include "PopupMenu.h"
#include "Building.h"
#include "BuildingResearchCenter.h"
#include "BuildingCommandPost.h"
#include "BuildingNeoridiumMine.h"
#include "Controls.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include <iostream>
#include <sstream>
#include <algorithm>
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

BehaviorArmy::FrontLine BehaviorArmy::frontLine;

// Allocate a new (Army) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorArmy::BehaviorArmy() :
		Behavior("BehaviorArmy")
{
	SetCurrentState(cBehaviorInvalid);
	SetNextState(cArmyNormal);

	if (!&frontLine)
	{
		frontLine = FrontLine();
	}
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
		// Load hud level
		hudSpace = Space::LoadLayer(side+2, "HUDLevel", true, true);
		hudSpace->GetLevelManager()->Update(0);

		// Move camera depending on army
		GameObject *camObj = hudSpace->GetGameObjectManager()->GetObjectByName("Camera");
		if (camObj) {
			if (side == BehaviorArmy::sRight)
				camObj->GetComponent<Camera>()->ChangePos({0.5f, 0.0f});
		}

		// Change HUDLevel cursor color
		GameObject *hudCursorObj = hudSpace->GetGameObjectManager()->GetObjectByName("Cursor");
		if (hudCursorObj) {
			if (side == BehaviorArmy::sRight)
				hudCursorObj->GetComponent<Sprite>()->SetModulateColor({0.f, 0.f, 1.f, 1.f});
		}

		// Move screen separator depending on army
		GameObject *separatorObj = hudSpace->GetGameObjectManager()->GetObjectByName("Separator");
		if (separatorObj) {
			if (side == BehaviorArmy::sRight)
				separatorObj->GetComponent<Transform>()->SetTranslation({-300, 0});
		}
		for (GameObject* obj : GetParent()->GetGameObjectManager()->GetObjectsByName("Army"))
		{
			BehaviorArmy* army = obj->GetComponent<BehaviorArmy>();
			if (army && army->side != side)
			{
				otherArmy = army;
			}
		}

		// Initialize tilemap
		tilemap = GetParent()->GetGameObjectManager()->GetObjectByName("Tilemap")->GetComponent<Tilemap>();

		// Initialize the front line struct.
		GameObject *frontLineObj = GetParent()->GetGameObjectManager()->GetObjectByName(frontLine.dispObjName.c_str());
		
		if (frontLineObj) 
			frontLine.transform = frontLineObj->GetComponent<Transform>();
		else 
			frontLine.transform = nullptr;

		// Initialize the funds struct.
		GameObject *fundsTextObj = hudSpace->GetGameObjectManager()->GetObjectByName(funds.dispObjName.c_str());

		if (fundsTextObj) {
			funds.text = fundsTextObj->GetComponent<Text>();
			funds.text->SetActive(true);
		} else
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

			neoridiumFundsDisplay = hudSpace->GetGameObjectManager()->GetObjectByName("LeftNeoridiumDisplay");
			neoridiumFundsDisplay->GetComponent<Text>()->SetActive(true);
			UpdateNeoridiumFundsText();
			break;
		case sRight:
			frontLine.start = frontLine.start < 0 ? tilemap->GetTilemapWidth() - 1 : frontLine.start;
			cursor.transform->SetTranslation(tilemap->GetPosOnScreen({ (float)tilemap->GetTilemapWidth() - 1, 0 }));

			neoridiumFundsDisplay = hudSpace->GetGameObjectManager()->GetObjectByName("RightNeoridiumDisplay");
			neoridiumFundsDisplay->GetComponent<Text>()->SetActive(true);
			UpdateNeoridiumFundsText();
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
		// Make sure that furthestX is always equal to the position of our furthest unit.
		if (side == sLeft)
			furthestX = -1;
		else
			furthestX = GridManager::GetInstance().width;

		for (GameObject* unit : BehaviorUnit::allUnits)
		{
			BehaviorUnit* bu = unit->GetComponent<BehaviorUnit>();

			int newX = (int)bu->GetGridPos().x;

			if (bu->GetArmy() == this)
			{
				if (side == sLeft)
				{
					if (newX > furthestX)
						furthestX = newX;
				}
				else
				{
					if (newX < furthestX)
						furthestX = newX;
				}
			}
		}

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

		bool buildingMenu = controls.gamepad->GetButtonTriggered(BTN_BUILDING_MENU);

		if (buildingMenu || (side == sRight && AEInputCheckTriggered('B'))) {
			if (PopupMenu::Exists(side)) PopupMenu::DestroyMenu(side);
			else PopupMenu::CreateMenu(this, PopupMenu::MenuType::Building);
		}
		if (PopupMenu::Exists(side)) PopupMenu::Update(side, *controls.gamepad, dt, cursorNode);
		else {
			// If the select button is pressed down this frame, set the select rectangle start positions.
			if (controls.gamepad->GetButtonTriggered(BTN_SELECT) || AEInputCheckTriggered(VK_RETURN)) { ///TODDO: Remove AEInput check
				rectStartPos = cursorNode;
			}

			// Are we selecting units?
			if (controls.gamepad->GetButton(BTN_SELECT) || AEInputCheckCurr(VK_RETURN) && rectStartPos) {
				if (cursorNode != rectEndPos) {
					rectEndPos = cursorNode;
					SelectUnits();
				}
			} else {
				if (selectedUnits.size() > 0)
				{
					if (cursorNode != prevTarget) {
						prevTarget = cursorNode;
						FindPath();
					}
				}
			}

			if (controls.gamepad->GetButtonReleased(BTN_SELECT) || AEInputCheckReleased(VK_RETURN)) {
				if (rectStartPos && rectStartPos == rectEndPos) {
					//Check if we are trying to select a building, and open the menu associated with that building if we are.
					for (GameObject* building : Building::allBuildings) {
						Building *buildingComp = building->GetChildComponent<Building>();
						if (buildingComp->army != this) continue;
						if (!buildingComp->IsBuilt()) continue;
						Transform *transform = building->GetComponent<Transform>();
						if (transform && GridManager::GetInstance().ConvertToGridPoint(transform->GetTranslation()) == cursorNode->gridPos()) {
							buildingComp->OpenMenu();
						}
					}
				}

				CalculateOffsets();
				rectStartPos = nullptr;
				rectEndPos = nullptr;
			}

			// If the move button is down, set the path for all selected units.
			if (controls.gamepad->GetButtonTriggered(BTN_MOVE)) {
				for (SelectedUnit &unit : selectedUnits)
				{
					unit.unit->SetPath(unit.path);
					unit.unit->SetNextState(BehaviorUnit::cUnitMove);
				}
				selectedUnits.clear();
			}
			if (controls.gamepad->GetButtonTriggered(BTN_TARGET))
			{
				for (SelectedUnit &unit : selectedUnits)
				{
					unit.unit->SetPath(unit.path);
					unit.unit->SetNextState(BehaviorUnit::cUnitAttack);
				}
				selectedUnits.clear();
			}
			if (controls.gamepad->GetButtonTriggered(BTN_GROUP))
			{
				FindPath(false);

				for (SelectedUnit &unit : selectedUnits)
				{
					unit.unit->SetPath(unit.path);
					unit.unit->SetNextState(BehaviorUnit::cUnitMove);
				}
				selectedUnits.clear();
			}
			break;
		}

		break;
	}

	GridManager& gm = GridManager::GetInstance();

	// Check if we need to update the position of the line.
	// True if our furthest unit is past the current line position but NOT past the other army's furthest unit.
	// False if we aren't past the current line position or we are, but we are also past the other army's furthest unit.
	if (side == sLeft)
	{
		// Are we past the line?
		if (gm.ConvertToWorldPoint(furthestX, 0).x > frontLine.transform->GetTranslation().x)
		{
			// Are we past the other army's furthest unit?
			if (furthestX < otherArmy->furthestX)
			{
				// Update line's position.
				frontLine.transform->SetTranslation({gm.ConvertToWorldPoint(furthestX, 0).x + tilemap->GetTileWidth()/2.f, frontLine.transform->GetTranslation().y});
				frontLine.pos = furthestX;
			}
		}
	}
	else
	{
		// Are we past the line?
		if (gm.ConvertToWorldPoint(furthestX, 0).x <= frontLine.transform->GetTranslation().x)
		{
			// Are we past the other army's furthest unit?
			if (furthestX > otherArmy->furthestX)
			{
				// Update line's position.
				frontLine.transform->SetTranslation({gm.ConvertToWorldPoint(furthestX, 0).x - tilemap->GetTileWidth()/2.f, frontLine.transform->GetTranslation().y});
				frontLine.pos = furthestX;
			}
		}
	}
}

bool BehaviorArmy::SelectedUnit::operator==(const SelectedUnit &other)
{
	if (unit == other.unit)
		return true;

	return false;
}

void BehaviorArmy::OnExit()
{
	switch (GetCurrentState()) {
	case cArmyNormal:
		break;
	}
}

void BehaviorArmy::Draw(Camera *cam) const
{
	if (!path.sprite) return;
	GridManager& gm = GridManager::GetInstance();

	Transform territoryTransform(0, 0);
	Vector2D frontLinePos = frontLine.transform->GetTranslation();
	Vector2D frontLineScale = frontLine.transform->GetScale();
	int height = gm.height * gm.tileHeight;
	switch (side) {
	case sLeft: {
		Vector2D mapTopLeft = gm.ConvertToWorldPoint(0, 0) - Vector2D((int)gm.tileWidth / 2.f, (int)gm.tileHeight / 2.f);
		territoryTransform.SetTranslation({ (frontLinePos.x - frontLineScale.x / 2) / 2 + mapTopLeft.x / 2, frontLinePos.y });
		territoryTransform.SetScale({ (frontLinePos.x - frontLineScale.x / 2) - mapTopLeft.x, (float)height });
		break;
	}
	case sRight: {
		Vector2D mapBottomRight = tilemap->GetTilemapScreenBottomRight();
		territoryTransform.SetTranslation({ (frontLinePos.x + frontLineScale.x / 2) / 2 + mapBottomRight.x / 2, frontLinePos.y });
		territoryTransform.SetScale({ mapBottomRight.x - (frontLinePos.x + frontLineScale.x / 2), (float)height });
		break;
	}
	}
	float alpha = path.sprite->GetAlpha();
	path.sprite->SetAlpha(0.5f);
	path.sprite->Draw(cam, territoryTransform);
	path.sprite->SetAlpha(alpha);
	if (selectedUnits.size() > 0)
		DrawPath(cam);
}

void BehaviorArmy::Load(rapidjson::Value & obj)
{
	if (obj.HasMember("Name") && obj["Name"].IsString()) {
		/*string path = "data\\";
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
		}*/
		armyTraits.SetArmyFile(obj["Name"].GetString());
		armyTraits.LoadArmyFile();
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

void BehaviorArmy::CreateUnit(unsigned unitId, Vector2D startPos)
{
	BehaviorUnit::Traits unitData = armyTraits.GetUnitData(unitId);
	if (funds.amount < unitData.GetCost()) return;
	if (!LegalSpawn(startPos)) return;
	funds.amount -= unitData.GetCost();
	UpdateFundsText();
	GameObject *go = GetParent()->GetGameObjectManager()->GetArchetype((string("Unit")+std::to_string(unitId+1)).c_str());
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
	if (!GridManager::GetInstance().GetNode(pos)->open)
		return false;
	if (pos.x < 0 || pos.x >= tilemap->GetTilemapWidth() || pos.y < 0 || pos.y >= tilemap->GetTilemapHeight())
		return false;
	return BehindFrontLine(pos);
}

void BehaviorArmy::UpdateNeoridiumFundsText()
{
	if(neoridiumFundsDisplay) neoridiumFundsDisplay->GetComponent<Text>()->SetText(("Neoridium: " + std::to_string((int)BuildingNeoridiumMine::GetNeoridium(side))).c_str());
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

void BehaviorArmy::FindPath(bool doOffset)
{
	Vector2D cursorGridPos = GridManager::GetInstance().ConvertToGridPoint(cursor.transform->GetTranslation());
	for (GameObject *buildingObj : Building::allBuildings) {
		Building *building = buildingObj->GetChildComponent<Building>();
		if (building->buildingType == Building::Teleporter && building->GetGridPos() == cursorGridPos)
			doOffset = false;
	}
	for (unsigned i = 0; i < selectedUnits.size(); i++)
	{
		SelectedUnit* unit = &selectedUnits[i];

		if (unit->unit->GetParent()->IsDestroyed())
		{
			selectedUnits.erase(std::remove(selectedUnits.begin(), selectedUnits.end(), *unit));
			i--;
			continue;
		}

		Vector2D pos = doOffset ? cursorGridPos + unit->offset : cursorGridPos;
		 
		if (pos.x < 0)
			pos.x = 0;
		if (pos.y < 0)
			pos.y = 0;
		if (pos.x >= GridManager::GetInstance().width)
			pos.x = (float)GridManager::GetInstance().width - 1;
		if (pos.y >= GridManager::GetInstance().height)
			pos.y = (float)GridManager::GetInstance().height - 1;

		unit->path = Pathfinder::FindPath(unit->unit->GetNode(), GridManager::GetInstance().GetNode((int)pos.x, (int)pos.y));
	}
}

// Function is used to figure out the midpoint of all selected units, then determines the offset for each unit from this point.
// Allows for the offset to be used when determining target position.
void BehaviorArmy::CalculateOffsets()
{
	// Figure out the mean point of all selected units.
	Vector2D min, max;
	min = { -1.f,-1.f };
	
	for (SelectedUnit& unit : selectedUnits)
	{
		Vector2D pos = unit.unit->GetGridPos();
		if ((unsigned)pos.x < (unsigned)min.x)
			min.x = pos.x;
		if (pos.x > max.x)
			max.x = pos.x;
		if ((unsigned)pos.y < (unsigned)min.y)
			min.y = pos.y;
		if (pos.y > max.y)
			max.y = pos.y;
	}

	Vector2D midpoint = min.Midpoint(max);
	midpoint.x = ceil(midpoint.x);
	midpoint.y = ceil(midpoint.y);

	SelectedUnit* closest = nullptr;
	float closestDist = -1.f;

	for (SelectedUnit& unit : selectedUnits)
	{
		float newDist = unit.unit->GetGridPos().Distance(midpoint);

		if (!closest || newDist < closestDist)
		{
			closest = &unit;
			closestDist = newDist;
		}
	}

	// Use the calculated point to determine offsets for all selected units.
	for (SelectedUnit& unit : selectedUnits)
	{
		unit.offset = unit.unit->GetGridPos() - closest->unit->GetGridPos();
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
	return armyTraits.GetUnitData(name_);
}

void BehaviorArmy::UpdateFundsText()
{
	if (funds.text) {
		funds.text->SetText(("Jaxium: " + std::to_string((unsigned)funds.amount)).c_str());
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
