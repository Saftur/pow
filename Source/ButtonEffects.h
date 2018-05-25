#pragma once

#include "Button.h"

class ButtonEffects {
public:
	static void List();

private:
	// Define game-side button effects here

	static void KillAllUnits(Button &button, float dt, int count = 0, ...);

	//Building Buttons
	static void CreateJaxiumMineEffect(Button &button, float dt, int count = 0, ...);
	static void CreateNeoridiumMineEffect(Button &button, float dt, int count = 0, ...);
	static void CreateResearchCenterEffect(Button &button, float dt, int count = 0, ...);
	static void CreateTeleporterEffect(Button &button, float dt, int count = 0, ...);

	//Research Buttons
	static void ResearchSpaceportEffect(Button &button, float dt, int count = 0, ...);
	static void ResearchVehicleDepotEffect(Button &button, float dt, int count = 0, ...);
	static void ResearchTurretEffect(Button &button, float dt, int count = 0, ...);
	static void ResearchTeleporterEffect(Button &button, float dt, int count = 0, ...);
	
	//Create Unit Buttons
	static void CreateUnit1Effect(Button &button, float dt, int count = 0, ...);
	static void CreateUnit2Effect(Button &button, float dt, int count = 0, ...);
	static void CreateUnit3Effect(Button &button, float dt, int count = 0, ...);
	static void CreateUnit4Effect(Button &button, float dt, int count = 0, ...);
};