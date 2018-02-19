#include "stdafx.h"

#include "Gamepad.h"

Gamepad::Gamepad() :
		gpNum(-1)
{
}

Gamepad::Gamepad(int gpNum_) :
		Gamepad()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT && gpNum == -1; i++) {
		if (XInputGetState(i, &state) == ERROR_SUCCESS) {
			if (!gpNum_) gpNum = i;
			else gpNum_--;
		}
	}
}

void Gamepad::Update()
{
	oldState = state;
	error = (XInputGetState(gpNum, &state) != ERROR_SUCCESS);
}

bool Gamepad::GetButton(WORD button)
{
	if (error) return false;
	return state.Gamepad.wButtons & button;
}

bool Gamepad::GetButtonTriggered(WORD button)
{
	if (error) return false;
	return (state.Gamepad.wButtons & button) && !(oldState.Gamepad.wButtons & button);
}

float Gamepad::GetAxis(Axis axis)
{
	if (error) return 0;
	switch (axis) {
	case aLTrigger:
		return TriggerToFloat(state.Gamepad.bLeftTrigger);
	case aRTrigger:
		return TriggerToFloat(state.Gamepad.bRightTrigger);
	case aLStickX:
		return StickToFloat(state.Gamepad.sThumbLX);
	case aLStickY:
		return StickToFloat(state.Gamepad.sThumbLY);
	case aRStickX:
		return StickToFloat(state.Gamepad.sThumbRX);
	case aRStickY:
		return StickToFloat(state.Gamepad.sThumbRY);
	default:
		return 0;
	}
}

int Gamepad::GetAxisNoDeadzone(Axis axis)
{
	if (error) return 0;
	switch (axis) {
	case aLTrigger:
		return state.Gamepad.bLeftTrigger;
	case aRTrigger:
		return state.Gamepad.bRightTrigger;
	case aLStickX:
		return state.Gamepad.sThumbLX;
	case aLStickY:
		return state.Gamepad.sThumbLY;
	case aRStickX:
		return state.Gamepad.sThumbRX;
	case aRStickY:
		return state.Gamepad.sThumbRY;
	default:
		return 0;
	}
}

void Gamepad::SetStickDeadzone(int deadzone)
{
	if (deadzone >= 0 && deadzone < 32767)
		triggerDeadzone = deadzone;
}

void Gamepad::SetTriggerDeadzon(int deadzone)
{
	if (deadzone >= 0 && deadzone < 255)
		stickDeadzone = deadzone;
}

float Gamepad::StickToFloat(int s)
{
	if (s >= 0) {
		s -= stickDeadzone;
		if (s < 0) s = 0;
	} else {
		s += stickDeadzone;
		if (s > 0) s = 0;
	}
	return (float)s / (32767 - stickDeadzone);
}

float Gamepad::TriggerToFloat(int t)
{
	if (t >= 0) {
		t -= triggerDeadzone;
		if (t < 0) t = 0;
	} else {
		t += triggerDeadzone;
		if (t > 0) t = 0;
	}
	return (float)t / (255 - triggerDeadzone);
}
