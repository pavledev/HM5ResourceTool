#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../ZString.hpp"
#include "../ZBinarySerializer.hpp"

namespace IOI
{
	struct SEntityTemplatePropertyAlias
	{
		ZString sAliasName;
		int entityID;
		ZString sPropertyName;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SEntityTemplatePropertyAlias* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
