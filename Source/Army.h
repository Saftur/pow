#pragma once

#include <vector>
using std::vector;

class GameObject;

class Army {
public:
	struct Unit {
		enum Ability { NONE, ARMOR, BOW, BUILDER };

		char name[100];
		int hp;
		int damage;
		int speed;
		Ability ability;
	};

	Army(const char *name);
	~Army();

	void AddUnit(Unit *unit);
	const char *GetName() const;
	Unit *GetUnit(const char *name);

private:
	const char *name;
	vector<Unit*> units;
};