#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

namespace IOI
{
	class ZBinarySerializer;

	struct SColorRGB
	{
		float r;
		float g;
		float b;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SColorRGB* DeserializeFromJSON(const rapidjson::Value& object);
		bool operator==(const SColorRGB& other);
	};
}
