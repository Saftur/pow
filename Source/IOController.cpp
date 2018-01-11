#include "stdafx.h"
#include "IOController.h"

using namespace std;

void IOController::LoadArmy(Army* army)
{
	string path = DATAPATH;
	path += army->GetName();

	ifstream workingFile(path);

	string line = "";
	Army::Unit* unit;
	int index = 0;

	while (getline(workingFile, line))
	{
		switch (index)
		{
		case 0:
			unit = army->Unit1;
			break;
		}

		stringstream reader (line);

		reader >> army->Unit1->hp;
		reader >> army->Unit1->damage;
		reader >> army->Unit1->speed;
		reader >> army->Unit1->ability;

		++index;
	}
}

void IOController::SaveArmy(Army& army)
{

}

IOController& IOController::GetInstance()
{
	static IOController cont;

	return cont;
}