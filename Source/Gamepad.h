#pragma once

#include <windows.h>
#include <xinput.h>

#define MAX_GAMEPADS 4

class Gamepad {
public:
	enum Button {
		bUnbound,
		bA = XINPUT_GAMEPAD_A, bB = XINPUT_GAMEPAD_B, bX = XINPUT_GAMEPAD_X, bY = XINPUT_GAMEPAD_Y,
		bDpadLeft = XINPUT_GAMEPAD_DPAD_LEFT, bDpadRight = XINPUT_GAMEPAD_DPAD_RIGHT,
		bDpadUp = XINPUT_GAMEPAD_DPAD_UP, bDpadDown = XINPUT_GAMEPAD_DPAD_DOWN,
		bLShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER, bRShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		bLThumb = XINPUT_GAMEPAD_LEFT_THUMB, bRThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		bBack = XINPUT_GAMEPAD_BACK, bStart = XINPUT_GAMEPAD_START,
		bLTrigger = (int)XINPUT_GAMEPAD_Y + 1, bRTrigger = bLTrigger + 1
	};
	enum Axis {
		aUnbound, aLTrigger = bLTrigger, aRTrigger, aLStickX, aLStickY, aRStickX, aRStickY
	};

	bool Connected();
	void Update();

	bool GetButton(int button);
	bool GetButtonTriggered(int button);
	bool GetButtonReleased(int button);
	float GetAxis(int axis);
	float GetAxisLastFrame(int axis);
	int GetAxisNoDeadzone(int axis);

	void SetStickDeadzone(int deadzone);
	void SetTriggerDeadzone(int deadzone);

	static Gamepad &GetGamepad(unsigned index);
	static void UpdateAll();

private:
	Gamepad();

	void SetGpNum(int gpNum);

	float StickToFloat(int s);
	float TriggerToFloat(int t);

	int gpNum;
	bool error;
	XINPUT_STATE oldState;
	XINPUT_STATE state;

	float triggerButtonPress = 0.9f;
	int triggerDeadzone = 5;
	int stickDeadzone = 3500;

	static Gamepad gamepads[MAX_GAMEPADS];
};