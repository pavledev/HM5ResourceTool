#pragma once

#include "TArray.hpp"
#include "TFixedArray.hpp"

namespace IOI
{
	class ZBinarySerializer;

	class ZCurve
	{
	public:
		TArray<TFixedArray<float, 8>> data;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
		static ZCurve* DeserializeFromJSON(const rapidjson::Value& object);
		bool operator==(const ZCurve& other);
	};
}
