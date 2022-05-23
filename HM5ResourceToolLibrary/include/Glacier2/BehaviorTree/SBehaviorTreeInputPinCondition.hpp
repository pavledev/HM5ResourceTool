#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../ZString.hpp"
#include "../ZBinarySerializer.hpp"

namespace IOI
{
	struct SBehaviorTreeInputPinCondition
	{
		ZString m_sName;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SBehaviorTreeInputPinCondition* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
