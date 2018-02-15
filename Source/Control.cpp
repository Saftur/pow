#include "stdafx.h"

#include "Control.h"
#include <AEEngine.h>


Control::Control(Type type_, int val_) :
		type(type_), val(val_)
{
}

bool Control::CheckHeld()
{
	switch (type) {
	case tKeyboard:
		return AEInputCheckCurr((u8)val);
	default:
		return false;
	}
}

bool Control::CheckTriggered()
{
	switch (type) {
	case tKeyboard:
		return AEInputCheckTriggered((u8)val);
	default:
		return false;
	}
}

bool Control::CheckReleased()
{
	switch (type) {
	case tKeyboard:
		return AEInputCheckReleased((u8)val);
	default:
		return false;
	}
}
