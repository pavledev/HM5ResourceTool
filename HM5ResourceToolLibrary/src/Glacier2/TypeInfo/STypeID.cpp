#include "pch.h"
#include <Windows.h>
#include <format>
#include "Glacier2/TypeInfo/STypeID.hpp"

namespace IOI
{
	STypeID::STypeID()
	{
		flags = 0;
		typeNum = 0;
		pTypeInfo = nullptr;
		pSourceType = nullptr;
	}

	STypeID::~STypeID()
	{
		delete pTypeInfo;
		delete pSourceType;
	}

	void STypeID::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		writer.String(pTypeInfo->pszTypeName);
	}

	void STypeID::SerializeToMemory(ZBinarySerializer& binarySerializer, unsigned int offset)
	{
		unsigned int typeIndex = binarySerializer.TypeIdToIndex(this);

		binarySerializer.WriteToMemory(&typeIndex, sizeof(unsigned int), offset);

		binarySerializer.RecordOffsetForTypeIDReindexing(offset);
	}

	STypeID* STypeID::DeserializeFromJSON(const rapidjson::Value& object, const char* key)
	{
		static ZTypeRegistry typeRegistry = {};

		if (typeRegistry.IsEmpty())
		{
			char buffer[MAX_PATH];

			GetModuleFileNameA(NULL, buffer, MAX_PATH);

			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			std::string filePath = std::format("{}/STypeIDs.json", std::string(buffer).substr(0, pos));
			HM5ResourceTool::IO::ZJSONDeserializer jsonDeserializer = HM5ResourceTool::IO::ZJSONDeserializer(filePath);

			jsonDeserializer.Deserialize<ZTypeRegistry>(typeRegistry);
		}

		const char* typeName = object[key].GetString();

		return typeRegistry.GetTypeIDFromTypeName(typeName);
	}
}
