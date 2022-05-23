#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../TArray.hpp"
#include "SEntityTemplateProperty.hpp"

namespace IOI
{
	struct SCppEntity
	{
		int blueprintIndexInResourceHeader;
		TArray<SEntityTemplateProperty> propertyValues;

		std::string SerializeToJSON();
		void SerializeToMemory(ZBinarySerializer& binarySerializer);
		static SCppEntity* DeserializeFromJSON(const rapidjson::Document& document);
	};
}
