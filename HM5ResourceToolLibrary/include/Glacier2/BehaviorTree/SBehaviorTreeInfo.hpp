#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../TArray.hpp"
#include "SBehaviorTreeEntityReference.hpp"
#include "SBehaviorTreeInputPinCondition.hpp"
#include "../ZBinarySerializer.hpp"

namespace IOI
{
	struct SBehaviorTreeInfo
	{
		TArray<SBehaviorTreeEntityReference> m_references;
		TArray<SBehaviorTreeInputPinCondition> m_inputPinConditions;

		std::string SerializeToJSON();
		void SerializeToMemory(ZBinarySerializer& binarySerializer);
		static SBehaviorTreeInfo* DeserializeFromJSON(const rapidjson::Document& document);
	};
}
