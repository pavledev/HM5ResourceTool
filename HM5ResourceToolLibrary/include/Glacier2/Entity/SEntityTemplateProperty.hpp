#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../ZVariant.hpp"
#include "IO/ZJSONSerializer.hpp"
#include "../ZBinarySerializer.hpp"
#include "Registry/ZPropertyRegistry.hpp"
#include "Hash/ZCrc32Generator.hpp"

namespace IOI
{
	struct SEntityTemplateProperty
	{
		unsigned int nPropertyID;
		ZVariant value;

		__declspec(dllexport) SEntityTemplateProperty();
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SEntityTemplateProperty* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
