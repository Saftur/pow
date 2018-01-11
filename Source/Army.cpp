
#include "stdafx.h"
#include "Army.h"
#include "GameObjectManager.h"


Army::Army(const char *name) :
	name(name)
{

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

const char *Army::GetName() const {
	return name;
}

Army::Unit * Army::GetUnit(const char * name)
{
	for (Unit *unit : units) {
		if (strcmp(name, unit->name) == 0)
			return unit;
	}
	return nullptr;
}
