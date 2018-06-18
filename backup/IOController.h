#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Army.h"

#define DATAPATH "data\\"

class IOController
{
public:
	IOController();
	void LoadArmy(Army* army);
	void SaveArmy(Army& army);

	static IOController& GetInstance();
};