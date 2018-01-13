
#include "stdafx.h"
#include "Army.h"
#include "GameObjectManager.h"
#include "IOController.h"


Army::Army(const char *name) :
	name(name)
{
	IOController::GetInstance().LoadArmy(this);
}

Army::~Army() {
	for (Unit *unit : units) {
		delete unit;
	}
}

void Army::AddUnit(Unit * unit)
{
	units.push_back(unit);
}

void Army::RemoveUnit(const char * unitName)
{
	for (unsigned int i = 0; i < units.size(); i++) {
		if (strcmp(unitName, units[i]->name) == 0)
			units.erase(units.begin() + i);
	}
}

const char *Army::GetName() const {
	return name;
}

/*const char ** Army::GetUnitNames() const
{
	const char *names[100];
	for (unsigned int i = 0; i < units.size(); i++) {
		names[i] = units[i]->name;
	}
	return names;
}*/

/*void Army::CloneUnit(const char * unitName, Vector2D pos) const
{
	Unit *unit = GetUnit(unitName);
	if (!unit) return;

	GameObject *gameObject = new GameObject(unitName);

	Transform *t = new Transform(0, 0);
}*/

Army::Unit * Army::GetUnit(const char * unitName) const
{
	for (Unit *unit : units) {
		if (strcmp(unitName, unit->name) == 0)
			return unit;
	}
	return nullptr;
}
