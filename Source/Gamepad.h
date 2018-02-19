#pragma once

#include <windows.h>
#include <xinput.h>

class Gamepad {
public:
	enum Axis {
		aLTrigger, aRTrigger, aLStickX, aLStickY, aRStickX, aRStickY
	};
	enum Button {
		bA = XINPUT_GAMEPAD_A, bB = XINPUT_GAMEPAD_B, bX = XINPUT_GAMEPAD_X, bY = XINPUT_GAMEPAD_Y, 
		bDpadLeft = XINPUT_GAMEPAD_DPAD_LEFT, bDpadRight = XINPUT_GAMEPAD_DPAD_RIGHT, 
		bDpadUp = XINPUT_GAMEPAD_DPAD_UP, bDpadDown = XINPUT_GAMEPAD_DPAD_DOWN, 
		bLShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER, bRShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER, 
		bLThumb = XINPUT_GAMEPAD_LEFT_THUMB, bRThumb = XINPUT_GAMEPAD_RIGHT_THUMB, 
		bBack = XINPUT_GAMEPAD_BACK, bStart = XINPUT_GAMEPAD_START
	};

	Gamepad();
	Gamepad(int gpNum);

	void Update();
	bool GetButton(WORD button);
	bool GetButtonTriggered(WORD button);
	float GetAxis(Axis axis);
	int GetAxisNoDeadzone(Axis axis);

	void SetStickDeadzone(int deadzone);
	void SetTriggerDeadzon(int deadzone);

private:
	float StickToFloat(int s);
	float TriggerToFloat(int t);

	int gpNum;
	bool error = true;
	XINPUT_STATE oldState;
	XINPUT_STATE state;

	int triggerDeadzone = 5;
	int stickDeadzone = 2000;
};