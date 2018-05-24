#include "stdafx.h"
#include "QuitButton.h"

#include "Engine.h"

QuitButton::QuitButton() : Button("QuitButton") {
}

Component * QuitButton::Clone() const {
	return new QuitButton(*this);
}

void QuitButton::Click(float dt) {
	Engine::GetInstance().Quit();
}
