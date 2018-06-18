#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "Button.h"
#include "Rect.h"

class NewUnitButton : public Button {
public:
	NewUnitButton();
	void Load(rapidjson::Value &obj);
	Component *Clone() const;

	void ClickEffect(float dt);

private:
	unsigned unitCount;

	Rect area;
	Vector2D spacing;

	struct ObjData {
		string name;
		string archetype;
		Vector2D pos;
	};

	static const vector<ObjData> objData;
};
