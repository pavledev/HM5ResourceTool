#include "pch.h"
#include "Glacier2/Entity/SCppEntity.hpp"

namespace IOI
{
    std::string SCppEntity::SerializeToJSON()
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        writer.String("blueprintIndexInResourceHeader");
        writer.Int(blueprintIndexInResourceHeader);

        writer.String("propertyValues");
        propertyValues.SerializeToJSON(writer);

        writer.EndObject();

        return stringBuffer.GetString();
    }

    void SCppEntity::SerializeToMemory(ZBinarySerializer& binarySerializer)
    {
        unsigned int blueprintIndexInResourceHeaderOffset = offsetof(SCppEntity, blueprintIndexInResourceHeader);
        unsigned int propertyValuesOffset = offsetof(SCppEntity, propertyValues);

        binarySerializer.WriteToMemory(&blueprintIndexInResourceHeader, sizeof(int), blueprintIndexInResourceHeaderOffset);

        propertyValues.SerializeToMemory(binarySerializer, propertyValuesOffset);
    }

    SCppEntity* SCppEntity::DeserializeFromJSON(const rapidjson::Document& document)
    {
        SCppEntity* cppEntity = new SCppEntity();

        cppEntity->blueprintIndexInResourceHeader = document["blueprintIndexInResourceHeader"].GetInt();
        cppEntity->propertyValues = *TArray<SEntityTemplateProperty>::DeserializeFromJSON(document["propertyValues"].GetArray());

        return cppEntity;
    }
}
