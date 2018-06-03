#pragma once

#include "Component.h"

class GameMain : public Component {
public:
	GameMain();

	void Load(rapidjson::Value &obj);

	Component *Clone() const;

	void Update(float dt);

	void TogglePaused();

private:
	bool paused = false; //Is the game paused?
	bool justLoaded = true;
};
