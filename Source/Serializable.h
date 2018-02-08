#pragma once

#include <Document.h>

class Serializable
{
public:
	Serializable(const char* typeStr);
	virtual void Load(const rapidjson::Value& obj);
private:
	const char* typeStr;
};