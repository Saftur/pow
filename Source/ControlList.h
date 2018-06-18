#pragma once

#include <map>
using std::map;
#include <string>
using std::string;

#include "Serializable.h"

class ControlList : public Serializable {
public:
	ControlList();

	void SetControl(const char *name, int control);
	int GetControl(const char *name);

	int operator[](const char *name);

	void Load(rapidjson::Value& obj);

private:
	ControlList *parent;
	map<string, int> controls;
	map<string, ControlList> categories;

	int& GetControlRef(const char *name);
	int* FindOtherControl(int control, bool localOnly=false);

	static int GetControlFromName(const char *name);
};