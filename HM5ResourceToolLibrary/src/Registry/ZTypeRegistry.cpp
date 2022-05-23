#include "pch.h"
#include "Registry/ZTypeRegistry.hpp"

void ZTypeRegistry::DeserializeFromJSON(rapidjson::Document& document, ZTypeRegistry& typeRegistry)
{
	const rapidjson::Value& typeIDs = document["typeIDs"];

	for (rapidjson::Value::ConstValueIterator it = typeIDs.Begin(); it != typeIDs.End(); ++it)
	{
		const rapidjson::Value& object = it->GetObj();
		const rapidjson::Value& object2 = object["pTypeInfo"].GetObj();

		IOI::STypeID* typeID = new IOI::STypeID();

		typeID->flags = object["flags"].GetUint();
		typeID->typeNum = object["typeNum"].GetUint();

		const char* typeName = object2["pszTypeName"].GetString();
		const unsigned int typeSize = object2["m_nTypeSize"].GetUint();
		const unsigned int typeAlignment = object2["m_nTypeAlignment"].GetUint();
		const unsigned int typeInfoFlags = object2["m_nTypeInfoFlags"].GetUint();

		IOI::IType* type = new IOI::IType(typeSize, typeAlignment, typeInfoFlags, typeName);

		typeID->pTypeInfo = type;

		typeRegistry.typeIDs.insert(std::make_pair(typeName, typeID));
	}
}

IOI::STypeID* ZTypeRegistry::GetTypeIDFromTypeName(const std::string& typeName)
{
	IOI::STypeID* typeID = nullptr;
	auto it = typeIDs.find(typeName);

	if (it != typeIDs.end())
	{
		typeID = it->second;
	}

	return typeID;
}

bool ZTypeRegistry::IsEmpty()
{
	return typeIDs.empty();
}
