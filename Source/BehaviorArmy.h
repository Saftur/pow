//------------------------------------------------------------------------------
//
// File Name:	BehaviorArmy.h
// Author(s):	Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Behavior.h"
#include "Tilemap.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
#include "Vector2D.h"
#include "Gamepad.h"
#include "Rendertext.h"
#include "Transform.h"
#include "ControlList.h"
#include "GridManager.h"
#include "BehaviorUnit.h"

typedef class Sprite Sprite;

using Node = GridManager::Node;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// An example of the enums to be defined in BehaviorArmy.cpp.
#if 0
enum ArmyState
{
};
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorArmy.h
class BehaviorArmy : public Behavior
{
public:

	// Army side
	enum Side {
		sIllegal, sLeft, sRight
	};

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Army) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorArmy();

	// Push the army's front line forward
	// Params:
	//  pos = The position of the unit pushing forward
	void PushFrontLine(Vector2D pos);

	// Get the army's side
	// Returns:
	//  The army's side
	Side GetSide();
	// Increase number of units in army
	void IncreaseUnits();
	// Decrease number of units in army
	void DescreaseUnits();
	// Get the amount of funds
	// Returns:
	//  The amount of funds
	float GetFunds();
	// Take money from funds
	// If there isn't enough, doesn't take
	// anything and returns false
	// Params:
	//  amount = amount to take
	// Returns:
	//  If there was enough
	bool TakeFromFunds(float amount);
	// Add money to funds
	// Params:
	//  amount = amount to add
	void AddToFunds(float amount);

	// Create a unit
	// Params:
	//  unitName = Name of unit to spawn
	//  startPos = Start position of unit
	//  path     = Starting path
	void CreateUnit(const char *unitName, Vector2D startPos);
	// Is spawn position "legal"
	// Params:
	//  pos = Spawn pos
	// Returns:
	//  Whether or not it's legal
	bool LegalSpawn(Vector2D pos);

	void UpdateNeoridiumFundsText();

	// 0 = cost over time
	// 1 = higher train cost for bigger army
	static const int costType = -1;

private:
	// Clone an advanced behavior and return a pointer to the cloned object.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	//   parent = A reference to the parent object (the object that owns this component).
	// Returns:
	//   A pointer to an advanced behavior.
	Component* Clone() const;

	// Initialize the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	void OnEnter();

	// Update the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	//	 dt = Change in time (in seconds) since the last game loop.
	void OnUpdate(float dt);

	void OnExit();

	void Draw(Camera *cam) const;

	void Load(rapidjson::Value& obj);

	// Is position behind army's front line
	// Params:
	//  pos = Position to check
	// Returns:
	//  Whether or not it's behind the front line
	bool BehindFrontLine(Vector2D pos);
	// Select unit under cursor
	// Params:
	//  curspos  = Position of cursor
	//  deselect = Deselect instead
	void SelectUnits();
	void FindPath(bool doOffset = true);
	// Draw the currently editing path
	void DrawPath(Camera *cam) const;

	// Get unit data by name of unit
	// Params:
	//  name = name of unit
	// Returns:
	//  unit data
	BehaviorUnit::Traits GetUnitData(const char *name) const;
	// Update on-screen funds text
	void UpdateFundsText();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Type of controls
	// TODO keyboard controls not yet implemented
	//enum ControlType {ctKeyboard, ctGamepad};


	// List of unit types
	vector<BehaviorUnit::Traits> unitTypes;
	// Side of army
	Side side;
	// Number of units alive from army
	unsigned numUnits;

	// Funds data
	struct {
		// Current funds
		float amount;
		// Starting funds
		unsigned startAmount;
		// Name of funds display object
		string dispObjName;
		// Text component of funds display object
		Text *text;
	} funds;

	GameObject* neoridiumFundsDisplay = nullptr; //Text object to display Neoridium.

	// Pointer to tilemap
	Tilemap *tilemap;

	// Front line data
	struct {
		// Current position of front line
		int pos;
		// Start position of front line
		int start;
		// Name of front line display object
		string dispObjName;
		// Pointer to Transform component of front line display object
		Transform *transform;
	} frontLine;

	// Controls data
	struct {
		// Player's gamepad
		Gamepad *gamepad;
		// List of controls for army
		ControlList list;
	} controls;

	// Cursor data
	struct {
		// Name of cursor display object
		string objName;
		// Transform component of cursor display object
		Transform *transform;
		// Sprite component of cursor display object
		Sprite *sprite;
	} cursor;

	// Pointer to camera position
	Vector2D *camera;
	
	struct SelectedUnit
	{
		BehaviorUnit* unit;	// The unit
		vector<Node*> path;	// The unit's path
		Vector2D offset;	// An offset from the move target
		bool operator==(const SelectedUnit &other);
	};

	// Editing data
	vector<SelectedUnit> selectedUnits;

	// Selection rectangle positions
	Node* rectStartPos;
	Node* rectEndPos;

	// Node targetted previous frame
	Node* prevTarget;

	// The debug path
	vector<GameObject*> debugPath;

	// Path drawing
	struct {
		string lineDispName;
		Sprite *sprite;
		Transform *transform;
	} path;

	void CalculateOffsets();
};

//------------------------------------------------------------------------------
