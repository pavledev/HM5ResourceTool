#pragma once

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "SVector3.hpp"

using namespace rapidjson;

namespace IOI
{
	struct SMatrix33
	{
		union
		{
			struct
			{
				SVector3 XAxis;
				SVector3 YAxis;
				SVector3 ZAxis;
			};

			struct
			{
				float m11;
				float m12;
				float m13;
				float m21;
				float m22;
				float m23;
				float m31;
				float m32;
				float m33;
			};

			float v[9];
			SVector3 r[3];
		};

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static SMatrix33* DeserializeFromJSON(const rapidjson::Value& object);
		bool operator==(const SMatrix33& other);
		SVector3 ConvertRotationMatrixToEulerAngles();
		static SMatrix33 ConvertEulerAnglesToRotationMatrix(const SVector3& vector);
	};
}
