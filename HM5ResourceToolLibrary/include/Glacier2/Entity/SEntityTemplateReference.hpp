#pragma once

#include <cstddef>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "../ZString.hpp"

using namespace rapidjson;

namespace IOI
{
	class ZBinarySerializer;

	struct SEntityTemplateReference
	{
		int entityIndex;
		ZString exposedEntity;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SEntityTemplateReference* DeserializeFromJSON(const rapidjson::Value& object);
		bool operator==(const SEntityTemplateReference& other);
	};
}
