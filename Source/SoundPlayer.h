#pragma once
#include "Behavior.h"
#include <string>
#include "Component.h"

typedef std::string string;

class SoundPlayer : public Component
{
public:
	SoundPlayer();
	Component* Clone() const;

	// Update the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);


	void Load(rapidjson::Value& obj);
private:
	string soundToPlay;
};