#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

using namespace rapidjson;

namespace IOI
{
	class ZBinarySerializer;

	struct SVector2
	{
		union
		{
			struct
			{
				float x;
				float y;
			};

			float v[2];
		};

		SVector2() = default;
		SVector2(const float x, const float y);
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SVector2* DeserializeFromJSON(const rapidjson::Value& object);
		bool operator==(const SVector2& other);
	};
}
