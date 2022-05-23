#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../TArray.hpp"
#include "SCppEntitySubsetInfo.hpp"
#include "IO/ZJSONSerializer.hpp"
#include "../TypeInfo/STypeID.hpp"

namespace IOI
{
	struct SCppEntityBlueprint
	{
		STypeID* typeName;
		TArray<SCppEntitySubsetInfo> subsets;

		std::string SerializeToJSON();
		void SerializeToMemory(ZBinarySerializer& binarySerializer);
		static SCppEntityBlueprint* DeserializeFromJSON(const rapidjson::Document& document);
	};
}
