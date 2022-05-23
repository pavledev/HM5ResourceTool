#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../ZString.hpp"
#include "../ZBinarySerializer.hpp"

namespace IOI
{
	struct SBehaviorTreeEntityReference
	{
		bool m_bList;
		ZString m_sName;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SBehaviorTreeEntityReference* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
