#include "stdafx.h"
#include "RestartButton.h"

#include "Engine.h"
#include "Space.h"

RestartButton::RestartButton() : Button("RestartButton") {
}

Component * RestartButton::Clone() const {
	return new RestartButton(*this);
}

void RestartButton::Click(float dt) {
	Engine::GetInstance().SetPaused(false);
	Space::GetLayer(0)->GetLevelManager()->Restart();
	if (Space::GetLayerCount() > 1) {
		PopupMenu::Shutdown();
		for (unsigned i = 1; i < Space::GetLayerCount(); i++) Space::DeleteLayer(i);
	}
}
