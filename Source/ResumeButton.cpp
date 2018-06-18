#include "stdafx.h"
#include "ResumeButton.h"

#include "GameObjectManager.h"
#include "GameObject.h"
#include "GameMain.h"

ResumeButton::ResumeButton() : Button("ResumeButton") {
}

Component * ResumeButton::Clone() const {
    return new ResumeButton(*this);
}

void ResumeButton::ClickEffect(float dt) {
    GetParent()->GetGameObjectManager()->GetObjectByName("MainObject")->GetComponent<GameMain>()->TogglePaused();
}
