#include "stdafx.h"
#include "IOController.h"

using namespace std;

void IOController::LoadArmy(Army* army)
{
	string path = DATAPATH;
	path += army->GetName();
	path += ".ini";

	ifstream workingFile(path);

	string line = "";
	Army::Unit* unit;
	int index = 0;

	while (getline(workingFile, line))
	{
		unit = new Army::Unit();

		stringstream reader (line);

		reader >> unit->name;
		reader >> unit->hp;
		reader >> unit->damage;
		reader >> unit->speed;
		reader >> *((int*)&(unit->ability));

		army->AddUnit(unit);
		++index;
	}
}

void IOController::SaveArmy(Army& army)
{
	UNREFERENCED_PARAMETER(army);
}

IOController& IOController::GetInstance()
{
	static IOController cont;

	return cont;
}

IOController::IOController() {}