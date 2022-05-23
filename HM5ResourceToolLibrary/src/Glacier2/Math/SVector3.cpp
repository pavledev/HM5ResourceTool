#include "pch.h"
#include "Glacier2/Math/SVector3.hpp"
#include "Glacier2/ZBinarySerializer.hpp"

namespace IOI
{
    SVector3::SVector3(const float x, const float y, const float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void SVector3::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("x");
        writer.Double(x);

        writer.String("y");
        writer.Double(y);

        writer.String("z");
        writer.Double(z);

        writer.EndObject();
    }

    void SVector3::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int xOffset = offset + offsetof(SVector3, x);
        unsigned int yOffset = offset + offsetof(SVector3, y);
        unsigned int zOffset = offset + offsetof(SVector3, z);

        binarySerializer.WriteToMemory(&x, sizeof(float), xOffset);
        binarySerializer.WriteToMemory(&y, sizeof(float), yOffset);
        binarySerializer.WriteToMemory(&z, sizeof(float), zOffset);
    }

    SVector3* SVector3::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SVector3* vector = new SVector3();

        vector->x = object["x"].GetFloat();
        vector->y = object["y"].GetFloat();
        vector->z = object["z"].GetFloat();

        return vector;
    }

    bool SVector3::operator==(const SVector3& other)
    {
        return x == other.x && y == other.y && z == other.z;
    }
}
