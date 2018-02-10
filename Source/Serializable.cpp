#include "stdafx.h"

#include "Serializable.h"
#include "Trace.h"

Serializable::Serializable(const char* typeStr)
	: typeStr(typeStr){}

void Serializable::Load(rapidjson::Value& obj)
{
	UNREFERENCED_PARAMETER(obj);

	Trace::GetInstance().GetStream() << "Serializable " << typeStr << ": load" << std::endl;
}