#pragma once
#include <string>
#include <fstream>
#include <sstream>

#define DATAPATH "data\\"

class Army
{
public:
	struct Unit
	{
		enum Ability {NONE, ARMOR, BOW, BUILDER};

		int hp;
		int damage;
		int speed;
		int ability;
	};

	Unit* Unit1;

	const char* GetName() const;

private:
	const char* name;
};

class IOController
{
public:
	void LoadArmy(Army* army);
	void SaveArmy(Army& army);

	static IOController& GetInstance();
private:
	IOController();
};