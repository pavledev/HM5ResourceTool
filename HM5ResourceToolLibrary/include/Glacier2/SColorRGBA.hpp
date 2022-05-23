#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

namespace IOI
{
	class ZBinarySerializer;

	struct SColorRGBA
	{
		float r;
		float g;
		float b;
		float a;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SColorRGBA* DeserializeFromJSON(const rapidjson::Value& object);
		bool operator==(const SColorRGBA& other);
	};
}
