#include "pch.h"
#include "Glacier2/Math/SVector4.hpp"
#include "Glacier2/ZBinarySerializer.hpp"

namespace IOI
{
	SVector4::SVector4(const float x, const float y, const float z, const float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	void SVector4::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		writer.StartObject();

		writer.String("x");
		writer.Double(x);

		writer.String("y");
		writer.Double(y);

		writer.String("z");
		writer.Double(z);

		writer.String("w");
		writer.Double(w);

		writer.EndObject();
	}

	void SVector4::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
	{
		unsigned int xOffset = offset + offsetof(SVector4, x);
		unsigned int yOffset = offset + offsetof(SVector4, y);
		unsigned int zOffset = offset + offsetof(SVector4, z);
		unsigned int wOffset = offset + offsetof(SVector4, w);

		binarySerializer.WriteToMemory(&x, sizeof(float), xOffset);
		binarySerializer.WriteToMemory(&y, sizeof(float), yOffset);
		binarySerializer.WriteToMemory(&z, sizeof(float), zOffset);
		binarySerializer.WriteToMemory(&w, sizeof(float), wOffset);
	}

	SVector4* SVector4::DeserializeFromJSON(const rapidjson::Value& object)
	{
		SVector4* vector = new SVector4();

		vector->x = object["x"].GetFloat();
		vector->y = object["y"].GetFloat();
		vector->z = object["z"].GetFloat();
		vector->w = object["w"].GetFloat();

		return vector;
	}
}
