#pragma once

#include <string>
using std::string;
#include "BehaviorUnit.h"

class ArmyTraits : public Component {
public:
	ArmyTraits();
	ArmyTraits(const char *armyFile);

	Component *Clone() const;

	void SetArmyFile(string armyFile);
	void SetArmyFile(const char *armyFile);
	void LoadArmyFile();
	void SaveArmyFile();

	BehaviorUnit::Traits *NewUnitData(string name);
	BehaviorUnit::Traits *SetUnitData(string name, BehaviorUnit::Traits traits);
	BehaviorUnit::Traits GetUnitData(string name) const;

	string armyFile;

	vector<BehaviorUnit::Traits> unitTypes;
};
