#include "stdafx.h"

#include "CompList.h"
#include "LevelManager.h"

#include "GameMain.h"

#include "ResumeButton.h"

#define NEWCOMP(name, comp) LevelManager::AddComponentType(name, comp)
void CompList::List() {
	//Level main components.
	NEWCOMP("GameMain", new GameMain());

	//Buttons
	NEWCOMP("ResumeButton", new ResumeButton());
}
#undef NEWCOMP
