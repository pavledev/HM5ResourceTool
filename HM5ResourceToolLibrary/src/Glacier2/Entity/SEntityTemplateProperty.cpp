#include "pch.h"
#include <Windows.h>
#include <format>
#include "Glacier2/Entity/SEntityTemplateProperty.hpp"

namespace IOI
{
    SEntityTemplateProperty::SEntityTemplateProperty()
    {
        nPropertyID = 0;
        value = ZVariant();
    }

    void SEntityTemplateProperty::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        static ZPropertyRegistry propertyRegistry;

        if (propertyRegistry.IsEmpty())
        {
            char buffer[MAX_PATH];

            GetModuleFileNameA(NULL, buffer, MAX_PATH);

            std::string::size_type pos = std::string(buffer).find_last_of("\\/");
            std::string filePath = std::format("{}/Properties.json", std::string(buffer).substr(0, pos));
            HM5ResourceTool::IO::ZJSONDeserializer jsonDeserializer = HM5ResourceTool::IO::ZJSONDeserializer(filePath);

            jsonDeserializer.Deserialize<ZPropertyRegistry>(propertyRegistry);
        }

        writer.StartObject();

        writer.String("nPropertyID");
        writer.Uint(nPropertyID);

        writer.String("propertyName");
        writer.String(propertyRegistry.GetPropertyNameFromHash(nPropertyID).c_str());

        writer.String("value");
        value.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SEntityTemplateProperty::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int nPropertyIDOffset = offset + offsetof(SEntityTemplateProperty, nPropertyID);
        unsigned int valueOffset = offset + offsetof(SEntityTemplateProperty, value);

        binarySerializer.WriteToMemory(&nPropertyID, sizeof(unsigned int), nPropertyIDOffset);

        value.SerializeToMemory(binarySerializer, valueOffset);
    }

    SEntityTemplateProperty* SEntityTemplateProperty::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SEntityTemplateProperty* entityTemplateProperty = new SEntityTemplateProperty();

        entityTemplateProperty->nPropertyID = object["nPropertyID"].GetUint();
        entityTemplateProperty->value = *ZVariant::DeserializeFromJSON(object["value"].GetObj());

        return entityTemplateProperty;
    }
}
