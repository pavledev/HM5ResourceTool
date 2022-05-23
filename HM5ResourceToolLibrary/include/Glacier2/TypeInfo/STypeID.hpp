#pragma once

#include <string>
#include "IType.hpp"
#include "../ZBinarySerializer.hpp"
#include "Registry/ZTypeRegistry.hpp"
#include "IO/ZJSONDeserializer.hpp"

namespace IOI
{
	struct STypeID
	{
		unsigned short flags;
		unsigned short typeNum;
		IType* pTypeInfo;
		STypeID* pSourceType;

		STypeID();
		~STypeID();
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static STypeID* DeserializeFromJSON(const rapidjson::Value& object, const char* key);
	};
}
