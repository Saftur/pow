#include "stdafx.h"
#include "ChangeLevelButton.h"

#include "Space.h"

ChangeLevelButton::ChangeLevelButton() : Button("ChangeLevelButton") {
}

void ChangeLevelButton::Load(rapidjson::Value & obj) {
	Button::Load(obj);
	rapidjson::Value *tmp;

	if (obj.HasMember("Level") && (tmp = &obj["Level"])->IsString()) {
		levelName = tmp->GetString();
	}
	if (obj.HasMember("Layer") && (tmp = &obj["Layer"])->IsInt()) {
		layerNum = tmp->GetInt();
	} else layerNum = -1;
}

Component * ChangeLevelButton::Clone() const {
	return new ChangeLevelButton(*this);
}

void ChangeLevelButton::ClickEffect(float dt) {
	LevelManager *lm = layerNum >= 0 ? Space::GetLayer(layerNum)->GetLevelManager() : GetParent()->GetLevelManager();
	if (lm) {
		lm->SetNextLevel(levelName);
	}
}
