#include "Serializable.h"
#include "Trace.h"

Serializable::Serializable(const char* typeStr)
	: typeStr(typeStr){}

void Serializable::Load(const rapidjson::Value& obj)
{
	Trace::GetInstance().GetStream() << "Serializable " << typeStr << ": load" << std::endl;
}