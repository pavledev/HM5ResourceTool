#include "pch.h"
#include "Glacier2/ZCurve.hpp"

namespace IOI
{
	void ZCurve::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		writer.StartObject();

		writer.String("data");
		data.SerializeToJSON(writer);

		writer.EndObject();
	}

	void ZCurve::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
	{
		unsigned int dataOffset = offset + offsetof(ZCurve, data);

		data.SerializeToMemory(binarySerializer, dataOffset);
	}

	ZCurve* ZCurve::DeserializeFromJSON(const rapidjson::Value& object)
	{
		ZCurve* curve = new ZCurve();

		curve->data = *TArray<TFixedArray<float, 8>>::DeserializeFromJSON(object["data"].GetArray());

		return curve;
	}

	bool ZCurve::operator==(const ZCurve& other)
	{
		return data == other.data;
	}
}
