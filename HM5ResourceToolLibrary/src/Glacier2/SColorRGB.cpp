#include "pch.h"
#include "Glacier2/SColorRGB.hpp"
#include "Glacier2/ZBinarySerializer.hpp"

namespace IOI
{
    void SColorRGB::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("r");
        writer.Double(r);

        writer.String("g");
        writer.Double(g);

        writer.String("b");
        writer.Double(b);

        writer.EndObject();
    }

    void SColorRGB::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int rOffset = offset + offsetof(SColorRGB, r);
        unsigned int gOffset = offset + offsetof(SColorRGB, g);
        unsigned int bOffset = offset + offsetof(SColorRGB, b);

        binarySerializer.WriteToMemory(&r, sizeof(float), rOffset);
        binarySerializer.WriteToMemory(&g, sizeof(float), gOffset);
        binarySerializer.WriteToMemory(&b, sizeof(float), bOffset);
    }

    SColorRGB* SColorRGB::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SColorRGB* colorRGB = new SColorRGB();

        colorRGB->r = object["r"].GetFloat();
        colorRGB->g = object["g"].GetFloat();
        colorRGB->b = object["b"].GetFloat();

        return colorRGB;
    }

    bool SColorRGB::operator==(const SColorRGB& other)
    {
        return r == other.r && g == other.g && b == other.b;
    }
}
