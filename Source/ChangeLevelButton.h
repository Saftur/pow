#pragma once

#include "Button.h"

class ChangeLevelButton : public Button {
public:
	ChangeLevelButton();
	void Load(rapidjson::Value &obj);
	Component *Clone() const;

	void ClickEffect(float dt);

private:
	string levelName;
	int layerNum;
};