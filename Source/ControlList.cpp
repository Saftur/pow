#include "stdafx.h"

#include "ControlList.h"
#include "Gamepad.h"

ControlList::ControlList() :
		Serializable("ControlList"), parent(nullptr)
{
}

void ControlList::SetControl(const char * name, int control)
{
	bool hasCategory = false;
	string category = name;
	string subControl;
	unsigned i;
	for (i = 0; i < category.size(); i++) {
		if (category.at(i) == '.') {
			subControl = category.substr(i+1);
			category = category.substr(0, i);
			hasCategory = true;
		}
	}
	if (hasCategory) {
		if (categories.count(category) == 0)
			categories[category].parent = this;
		categories[category].SetControl(subControl.c_str(), control);
	} else {
		int *other = FindOtherControl(control);
		if (other) {
			*other = controls[name];
		}
		controls[name] = control;
	}
}

int ControlList::GetControl(const char * name)
{
	bool hasCategory = false;
	string category = name;
	string subControl;
	unsigned i;
	for (i = 0; i < category.size(); i++) {
		if (category.at(i) == '.') {
			subControl = category.substr(i+1);
			category = category.substr(0, i);
			hasCategory = true;
		}
	}
	if (hasCategory) {
		if (categories.count(category) > 0)
			return categories[category].GetControl(subControl.c_str());
		else return Gamepad::bUnbound;
	} else if (controls.count(name) > 0)
		return controls[name];
	else return Gamepad::bUnbound;
}

int ControlList::operator[](const char * name)
{
	return GetControl(name);
}

void ControlList::Load(rapidjson::Value & obj)
{
	int control;
	for (rapidjson::Value::MemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); itr++) {
		if (itr->value.IsString()) {
			control = GetControlFromName(itr->value.GetString());
			if (control)
				SetControl(itr->name.GetString(), control);
		} else if (itr->value.IsObject()) {
			if (categories.count(itr->name.GetString()) == 0)
				categories[itr->name.GetString()].parent = this;
			categories[itr->name.GetString()].Load(itr->value);
		}
	}
}

int & ControlList::GetControlRef(const char * name)
{
	bool hasCategory = false;
	string category = name;
	string subControl;
	unsigned i;
	for (i = 0; i < category.size(); i++) {
		if (category.at(i) == '.') {
			subControl = category.substr(i+1);
			category = category.substr(0, i);
			hasCategory = true;
		}
	}
	if (hasCategory) {
		return categories[category].GetControlRef(subControl.c_str());
	} return controls[name];
}

int * ControlList::FindOtherControl(int control, bool localOnly)
{
	for (std::pair<const string, int> p : controls) {
		if (p.second == control) {
			return &(controls[p.first]);
		}
	}
	int *found;
	if (parent && (found = parent->FindOtherControl(control, true)))
		return found;
	if (localOnly) return nullptr;
	for (std::pair<const string, ControlList> p : categories) {
		found = p.second.FindOtherControl(control);
		if (found)
			return found;
	}
	return nullptr;
}

int ControlList::GetControlFromName(const char * name)
{
	if (strcmp(name, "LStickX") == 0)
		return Gamepad::aLStickX;
	if (strcmp(name, "LStickY") == 0)
		return Gamepad::aLStickY;
	if (strcmp(name, "RStickX") == 0)
		return Gamepad::aRStickX;
	if (strcmp(name, "RStickY") == 0)
		return Gamepad::aRStickY;
	if (strcmp(name, "LTrigger") == 0)
		return Gamepad::aLTrigger;
	if (strcmp(name, "RTrigger") == 0)
		return Gamepad::aRTrigger;
	if (strcmp(name, "A") == 0)
		return Gamepad::bA;
	if (strcmp(name, "B") == 0)
		return Gamepad::bB;
	if (strcmp(name, "X") == 0)
		return Gamepad::bX;
	if (strcmp(name, "Y") == 0)
		return Gamepad::bY;
	if (strcmp(name, "DpadUp") == 0)
		return Gamepad::bDpadUp;
	if (strcmp(name, "DpadDown") == 0)
		return Gamepad::bDpadDown;
	if (strcmp(name, "DpadLeft") == 0)
		return Gamepad::bDpadLeft;
	if (strcmp(name, "DpadRight") == 0)
		return Gamepad::bDpadRight;
	if (strcmp(name, "LShoulder") == 0)
		return Gamepad::bLShoulder;
	if (strcmp(name, "RShoulder") == 0)
		return Gamepad::bRShoulder;
	if (strcmp(name, "LThumb") == 0)
		return Gamepad::bLThumb;
	if (strcmp(name, "RThumb") == 0)
		return Gamepad::bRThumb;
	if (strcmp(name, "Back") == 0)
		return Gamepad::bBack;
	if (strcmp(name, "Start") == 0)
		return Gamepad::bStart;
	if (strcmp(name, "LTrigger") == 0)
		return Gamepad::bLTrigger;
	if (strcmp(name, "RTrigger") == 0)
		return Gamepad::bRTrigger;
	return Gamepad::bUnbound;
}
