#pragma once

#include <vector>
using std::vector;

#include "Rendertext.h"
#include "Transform.h"
#include "Sprite.h"

class TextBox : public Text {
public:
	TextBox();

	void Load(rapidjson::Value &obj);

	Component *Clone() const;

	void Update(float dt);

	void Draw(Camera *cam) const;

	virtual void EnterEffect();

private:
	bool focus;
	unsigned charCap;

	static vector<char> keys;
	static vector<char> keyVals;
	static vector<char> keyShiftVals;
};
