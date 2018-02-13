#pragma once

#include <Document.h>

class Serializable
{
public:
	Serializable(const char* typeStr);
	virtual void Load(rapidjson::Value& obj);
private:
	const char* typeStr;
};