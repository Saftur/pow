#include "stdafx.h"
#include "ArmyTraits.h"

#include <iostream>
#include <sstream>
using std::ifstream;
using std::ofstream;
using std::stringstream;

ArmyTraits::ArmyTraits() : Component("ArmyTraits") {
}

ArmyTraits::ArmyTraits(const char * armyFile) : Component("ArmyTraits") {
	SetArmyFile(armyFile);
	LoadArmyFile();
}

Component * ArmyTraits::Clone() const {
	return new ArmyTraits(*this);
}

void ArmyTraits::SetArmyFile(string armyFile) {
	this->armyFile = armyFile;
}

void ArmyTraits::SetArmyFile(const char * armyFile) {
	this->armyFile = armyFile;
}

void ArmyTraits::LoadArmyFile() {
	string path = "data\\";
	path += armyFile;
	path += ".ini";

	ifstream workingFile(path);

	string line = "";
	BehaviorUnit::Traits unit;

	unitTypes.clear();

	while (getline(workingFile, line))
	{
		unit = BehaviorUnit::Traits();

		stringstream reader (line);

		reader >> unit.name;
		reader >> unit.strength;
		reader >> unit.agility;
		reader >> unit.defense;
		reader >> unit.ability;
		reader >> unit.weapon;
		reader >> unit.item1;
		reader >> unit.item2;

		unitTypes.push_back(unit);
	}
}

void ArmyTraits::SaveArmyFile() {
	string path = "data\\";
	path += armyFile;
	path += ".ini";

	ofstream workingFile(path);

	for (BehaviorUnit::Traits unit : unitTypes) {
		workingFile << unit.name << ' ';
		workingFile << unit.strength << ' ';
		workingFile << unit.agility << ' ';
		workingFile << unit.defense << ' ';
		workingFile << unit.ability << ' ';
		workingFile << unit.weapon << ' ';
		workingFile << unit.item1 << ' ';
		workingFile << unit.item2;

		workingFile << std::endl;
	}
}

BehaviorUnit::Traits * ArmyTraits::NewUnitData(string name) {
	unitTypes.push_back(BehaviorUnit::Traits());
	unitTypes[unitTypes.size()-1].name = name;
	return &unitTypes[unitTypes.size()-1];
}

BehaviorUnit::Traits * ArmyTraits::SetUnitData(string name, BehaviorUnit::Traits traits) {
	for (BehaviorUnit::Traits &unit : unitTypes) {
		if (unit.name == name) {
			unit = traits;
			return &unit;
		}
	}
	BehaviorUnit::Traits *newUnit = NewUnitData(name);
	*newUnit = traits;
	return newUnit;
}

BehaviorUnit::Traits ArmyTraits::GetUnitData(string name) const {
	for (const BehaviorUnit::Traits &unit : unitTypes) {
		if (unit.name == name) {
			return unit;
		}
	}
	return BehaviorUnit::Traits({ -1 });
}
