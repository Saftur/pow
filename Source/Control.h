#pragma once

class Control {
public:
	enum Type {
		tKeyboard,
	};

	Control(Type type, int val);

	bool CheckHeld();
	bool CheckTriggered();
	bool CheckReleased();

private:
	Type type;
	int val;
};