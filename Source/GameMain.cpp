#include "stdafx.h"
#include "GameMain.h"

#include <AEEngine.h>
#include "Space.h"

GameMain::GameMain() : Component("GameMain") {
}

void GameMain::Load(rapidjson::Value & obj) {
	if (obj.HasMember("Paused") && obj["Paused"].IsTrue())
		paused = true;
}

Component * GameMain::Clone() const {
	return new GameMain(*this);
}

void GameMain::Update(float dt) {
	if (!justLoaded && AEInputCheckTriggered(VK_ESCAPE)) {
		TogglePaused();
	}
	justLoaded = false;
}

void GameMain::TogglePaused() {
	if (!paused)
		Space::LoadLayer(9, "PauseLevel", false, true);
	else Space::GetLayer(9)->GetLevelManager()->Quit();
}
