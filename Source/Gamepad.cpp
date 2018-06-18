#include "stdafx.h"

#include "Gamepad.h"

Gamepad Gamepad::gamepads[MAX_GAMEPADS];

Gamepad::Gamepad() :
		gpNum(-1), error(true)
{
}

void Gamepad::SetGpNum(int gpNum) {
	this->gpNum = gpNum;
	error = (XInputGetState(gpNum, &state) != ERROR_SUCCESS);
	oldState = state;
}

bool Gamepad::Connected() {
	return !error;
}

void Gamepad::Update()
{
	if (gpNum == -1) return;
	oldState = state;
	error = (XInputGetState(gpNum, &state) != ERROR_SUCCESS);
}

bool Gamepad::GetButton(int button)
{
	if (error) return false;
	if (button == bUnbound)
		return false;
	if (button == bLTrigger)
		return GetAxis(aLTrigger) >= triggerButtonPress;
	else if (button == bRTrigger)
		return GetAxis(aRTrigger) >= triggerButtonPress;
	return state.Gamepad.wButtons & button;
}

bool Gamepad::GetButtonTriggered(int button)
{
	if (error) return false;
	if (button == bUnbound)
		return false;
	if (button == bLTrigger)
		return (GetAxis(aLTrigger) >= triggerButtonPress) && (GetAxisLastFrame(aLTrigger) < triggerButtonPress);
	else if (button == bRTrigger)
		return (GetAxis(aRTrigger) >= triggerButtonPress) && (GetAxisLastFrame(aRTrigger) < triggerButtonPress);
	return (state.Gamepad.wButtons & button) && !(oldState.Gamepad.wButtons & button);
}

bool Gamepad::GetButtonReleased(int button)
{
	if (error) return false;
	if (button == bUnbound)
		return false;
	if (button == bLTrigger)
		return (GetAxis(aLTrigger) < triggerButtonPress) && (GetAxisLastFrame(aLTrigger) >= triggerButtonPress);
	else if (button == bRTrigger)
		return (GetAxis(aRTrigger) < triggerButtonPress) && (GetAxisLastFrame(aRTrigger) >= triggerButtonPress);
	return !(state.Gamepad.wButtons & button) && (oldState.Gamepad.wButtons & button);
}

float Gamepad::GetAxis(int axis)
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

float Gamepad::GetAxisLastFrame(int axis)
{
	if (error) return 0;
	switch (axis) {
	case aLTrigger:
		return TriggerToFloat(oldState.Gamepad.bLeftTrigger);
	case aRTrigger:
		return TriggerToFloat(oldState.Gamepad.bRightTrigger);
	case aLStickX:
		return StickToFloat(oldState.Gamepad.sThumbLX);
	case aLStickY:
		return StickToFloat(oldState.Gamepad.sThumbLY);
	case aRStickX:
		return StickToFloat(oldState.Gamepad.sThumbRX);
	case aRStickY:
		return StickToFloat(oldState.Gamepad.sThumbRY);
	default:
		return 0;
	}
}

int Gamepad::GetAxisNoDeadzone(int axis)
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

void Gamepad::SetTriggerDeadzone(int deadzone)
{
	if (deadzone >= 0 && deadzone < 255)
		stickDeadzone = deadzone;
}

Gamepad & Gamepad::GetGamepad(unsigned index) {
	if (gamepads[index].gpNum == -1)
		gamepads[index].SetGpNum(index);
	return gamepads[index];
}

void Gamepad::UpdateAll() {
	for (Gamepad &gamepad : gamepads) {
		gamepad.Update();
	}
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
