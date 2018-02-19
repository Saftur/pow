#include "stdafx.h"

#include "ButtonEffects.h"

#include "Button.h"
#include "Trace.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <vector>
using std::vector;

void ButtonEffects::List()
{
	// Add game-side button effects here using:
	// Button::AddClickEffect([name], [effect function]);
	Button::AddClickEffect("KillAllUnits", KillAllUnits);
}

void ButtonEffects::KillAllUnits(Button &button, float dt)
{
	vector<GameObject*> units = GameObjectManager::GetInstance().GetObjectsByName("Unit");
	for (GameObject* go : units)
		go->Destroy();
}
