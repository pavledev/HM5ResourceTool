#pragma once

#include <iostream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../TArray.hpp"
#include "STemplateSubEntity.hpp"
#include "../ZBinarySerializer.hpp"

namespace HM5ResourceTool
{
	class ZResource;
}

namespace IOI
{
	struct STemplateEntity
	{
		int blueprintIndexInResourceHeader;
		int rootEntityIndex;
		TArray<STemplateSubEntity> entityTemplates;

		std::string SerializeToJSON();
		void SerializeToMemory(ZBinarySerializer& binarySerializer);
		static STemplateEntity* DeserializeFromJSON(const rapidjson::Document& document);
	};
}
