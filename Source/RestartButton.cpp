#include "stdafx.h"
#include "RestartButton.h"

#include "Engine.h"
#include "Space.h"

RestartButton::RestartButton() : Button("RestartButton") {
}

Component * RestartButton::Clone() const {
	return new RestartButton(*this);
}

void RestartButton::ClickEffect(float dt) {
	Space::GetLayer(0)->GetLevelManager()->Restart();
	if (Space::GetLayerCount() > 1) {
		PopupMenu::Shutdown();
		for (unsigned i = 1; i < MAX_LAYERS; i++) {
			if (Space::GetLayer(i))
				Space::GetLayer(i)->GetLevelManager()->Quit();
		}
	}
}
