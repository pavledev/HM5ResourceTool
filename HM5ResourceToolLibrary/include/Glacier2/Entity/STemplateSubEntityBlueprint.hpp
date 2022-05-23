#pragma once

#include "../ZString.hpp"
#include "../TArray.hpp"
#include "../TPair.hpp"
#include "SEntityTemplatePropertyAlias.hpp"
#include "SEntityTemplateReference.hpp"
#include "SEntityTemplateEntitySubset.hpp"

namespace IOI
{
	struct STemplateSubEntityBlueprint
	{
		int parentIndex;
		int entityTypeResourceIndex;
		ZString entityName;
		TArray<SEntityTemplatePropertyAlias> propertyAliases;
		TArray<TPair<ZString, SEntityTemplateReference>> exposedEntities;
		TArray<TPair<ZString, int>> exposedInterfaces;
		TArray<TPair<ZString, SEntityTemplateEntitySubset>> entitySubsets;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static STemplateSubEntityBlueprint* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
