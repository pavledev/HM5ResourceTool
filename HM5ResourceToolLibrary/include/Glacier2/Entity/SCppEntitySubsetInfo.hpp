#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../ZString.hpp"
#include "../TypeInfo/STypeID.hpp"

namespace IOI
{
	struct SCppEntitySubsetInfo
	{
		ZString name;
		STypeID* type;
		unsigned int flags;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SCppEntitySubsetInfo* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
