//------------------------------------------------------------------------------
//
// File Name:	BehaviorArmy.h
// Author(s):	Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
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
#include "Control.h"


typedef class Sprite Sprite;

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
	struct UnitData {
		// NUMABILITIES isn't an actual ability, it's just a counter
		enum Ability { NONE, ARMOR, BOW, BUILDER, NUMABILITIES };

		int hp;
		int damage;
		int speed;
		Ability ability;
		BehaviorArmy *army;
		char name[100];
		Sprite* abilitySprite;
	};

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

	void PushFrontLine(Vector2D pos);

	Side GetSide();

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

	void Load(rapidjson::Value& obj);

	void CreateUnit(const char *unitName, Vector2D startPos, vector<Vector2D> path);
	bool LegalSpawn(Vector2D pos);
	bool BehindFrontLine(Vector2D pos);

	UnitData GetUnitData(const char *name) const;

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------


	Tilemap *tilemap;
	vector<UnitData> units;
	Side side;
	int frontLine, flStart;
	string flObjName;
	Transform *flTransform;
	vector<Control> controls;

	vector<Vector2D> path_;
};

//------------------------------------------------------------------------------