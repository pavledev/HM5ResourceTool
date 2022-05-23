#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../TArray.hpp"
#include "../TypeInfo/STypeID.hpp"

namespace IOI
{
	struct SEntityTemplateEntitySubset
	{
		STypeID* subsetType;
		TArray<int> entities;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SEntityTemplateEntitySubset* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
