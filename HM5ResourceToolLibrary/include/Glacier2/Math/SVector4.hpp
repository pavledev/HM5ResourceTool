#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "SVector3.hpp"

namespace IOI
{
	struct SVector4
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};

			struct
			{
				SVector3 n;
				float d;
			};

			float v[4];
		};

		SVector4() = default;
		SVector4(const float x, const float y, const float z, const float w);
		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SVector4* DeserializeFromJSON(const rapidjson::Value& object);
	};
}
