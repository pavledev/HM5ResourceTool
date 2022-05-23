#pragma once

#include "../TArray.hpp"
#include "SEntityTemplateProperty.hpp"

namespace IOI
{
	struct STemplateSubEntity
	{
		int parentIndex;
		int entityTypeResourceIndex;
		TArray<SEntityTemplateProperty> propertyValues;
		TArray<SEntityTemplateProperty> postInitPropertyValues;

		STemplateSubEntity();
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static STemplateSubEntity* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
