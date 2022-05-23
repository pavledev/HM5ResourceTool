#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../ZString.hpp"
#include "../ZBinarySerializer.hpp"

namespace IOI
{
	struct SEntityTemplatePinConnection
	{
		int fromID;
		int toID;
		ZString fromPinName;
		ZString toPinName;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SEntityTemplatePinConnection* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
