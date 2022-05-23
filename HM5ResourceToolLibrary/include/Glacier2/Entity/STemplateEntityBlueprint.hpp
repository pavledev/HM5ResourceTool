#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../TArray.hpp"
#include "STemplateSubEntityBlueprint.hpp"
#include "SEntityTemplatePinConnection.hpp"
#include "../TypeInfo/STypeID.hpp"
#include "IO/ZJSONSerializer.hpp"

namespace IOI
{
	struct STemplateEntityBlueprint
	{
		int rootEntityIndex;
		TArray<STemplateSubEntityBlueprint> entityTemplates;
		TArray<SEntityTemplatePinConnection> pinConnections;
		TArray<SEntityTemplatePinConnection> inputPinForwardings;
		TArray<SEntityTemplatePinConnection> outputPinForwardings;

		std::string SerializeToJSON();
		void SerializeToMemory(ZBinarySerializer& binarySerializer);
		static STemplateEntityBlueprint* DeserializeFromJSON(const rapidjson::Document& document);
	};
}
