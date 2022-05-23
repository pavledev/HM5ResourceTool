#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

using namespace rapidjson;

namespace IOI
{
	class ZBinarySerializer;

	struct SVector3
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};

			float v[3];
		};

		SVector3() = default;
		SVector3(const float x, const float y, const float z);
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SVector3* DeserializeFromJSON(const rapidjson::Value& object);
		bool operator==(const SVector3& other);
	};
}
