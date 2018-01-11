
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

Army::Unit * Army::GetUnit(const char * unitName)
{
	for (Unit *unit : units) {
		if (strcmp(unitName, unit->name) == 0)
			return unit;
	}
	return nullptr;
}
