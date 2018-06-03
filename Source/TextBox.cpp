#include "stdafx.h"
#include "TextBox.h"

#include <AEEngine.h>
#include "GameObject.h"
#include "Mesh.h"

vector<char> TextBox::keys;
vector<char> TextBox::keyVals;
vector<char> TextBox::keyShiftVals;

TextBox::TextBox() {
	name = "TextBox";

	if (keys.empty()) {
		for (char c = 'A'; c <= 'Z'; c++) {
			keys.push_back(c);
			keyVals.push_back(c-'A'+'a');
			keyShiftVals.push_back(c);
		}
		for (char c = '0'; c <= '9'; c++) {
			keys.push_back(c);
			keyVals.push_back(c);
			keyShiftVals.push_back(c);
		}
		keys.push_back(' ');
		keyVals.push_back(' ');
		keyShiftVals.push_back(' ');
	}
}

void TextBox::Load(rapidjson::Value & obj) {
	Text::Load(obj);

	if (obj.HasMember("CharCap") && obj["CharCap"].IsUint())
		charCap = obj["CharCap"].GetUint();
}

Component * TextBox::Clone() const {
	return new TextBox(*this);
}

void TextBox::Update(float dt) {
	if (!active) return;
	if (AEInputCheckTriggered(VK_LBUTTON)) {
		//Get the mouse position on screen.
		s32 mouseX;
		s32 mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);

		//Convert mouse screen position to world position.
		float worldX;
		float worldY;
		AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &worldX, &worldY);
		Vector2D mousePos = Vector2D(worldX, worldY);

		//Check if the mouse is within the bounds of this button.
		Vector2D boxScale = GetParent()->GetComponent<Transform>()->GetScale();
		Vector2D boxPos = GetParent()->GetComponent<Transform>()->GetTranslation();

		if (mousePos.X() > boxPos.X() - (boxScale.X() / 2) && mousePos.X() <= boxPos.X() + (boxScale.X() / 2)
			&& mousePos.Y() > boxPos.Y() - (boxScale.Y() / 2) && mousePos.Y() <= boxPos.Y() + (boxScale.Y() / 2)) {
			focus = true;
		} else {
			focus = false;
		}
	}

	if (focus) {
		bool shift = AEInputCheckCurr(VK_LSHIFT) || AEInputCheckCurr(VK_RSHIFT);
		for (unsigned i = 0; i < keys.size(); i++) {
			if (str.size() >= charCap)
				break;
			if (AEInputCheckTriggered(keys[i]))
				str += shift ? keyShiftVals[i] : keyVals[i];
		}
		if (str.size() > 0 && AEInputCheckTriggered(VK_BACK))
			str = str.substr(0, str.size()-1);
		if (AEInputCheckTriggered(VK_RETURN)) {
			EnterEffect();
			focus = false;
		}
	}
}

void TextBox::Draw(Camera * cam) const {
	Text::Draw(cam);
}

void TextBox::EnterEffect() {
}
