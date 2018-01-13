#pragma once

#include <vector>
#include "Sprite.h"
using std::vector;

class GameObject;

class Army {
public:
	struct Unit {
		enum Ability { NONE, ARMOR, BOW, BUILDER };

		int hp;
		int damage;
		int speed;
		Ability ability;
		char name[100];
		Sprite* abilitySprite;
	};

	Army(const char *name);
	~Army();
	
	void AddUnit(Unit *unit);
	void RemoveUnit(const char *unitName);
	const char *GetName() const;
	//const char **GetUnitNames() const;
	//void CloneUnit(const char *unitName, Vector2D pos) const;
	Unit *GetUnit(const char *unitName) const;

private:
	const char *name;
	vector<Unit*> units;
};