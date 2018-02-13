#include "stdafx.h"
//#include "LevelTimer.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string>
//
//LevelTimer::LevelTimer(Vector2D pos, Vector2D scale) : text("00:00:00", pos, scale) {
//	text.draw();
//}
//
//void LevelTimer::Update(float dt) {
//	time += dt;
//	
//	std::string timeText = "";
//
//	int minutes = ((int)time / 60) % 60;
//	int seconds = (int)time % 60;
//	int miliseconds = (int)((time - (int)time) * 100);
//
//	if (minutes < 10) timeText += "0";
//	timeText += std::to_string(minutes) + ":";
//	if (seconds < 10) timeText += "0";
//	timeText += std::to_string(seconds) + ":";
//	if (miliseconds < 10) timeText += "0";
//	timeText += std::to_string(miliseconds);
//
//	text.setText(timeText.c_str());
//	text.draw();
//}