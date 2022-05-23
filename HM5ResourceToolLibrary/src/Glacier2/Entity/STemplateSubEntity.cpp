#include "pch.h"
#include "Glacier2/Entity/STemplateSubEntity.hpp"

namespace IOI
{
    STemplateSubEntity::STemplateSubEntity()
    {
        parentIndex = 0;
        entityTypeResourceIndex = 0;
        propertyValues = TArray<SEntityTemplateProperty>();
        postInitPropertyValues = TArray<SEntityTemplateProperty>();
    }

    void STemplateSubEntity::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("parentIndex");
        writer.Int(parentIndex);

        writer.String("entityTypeResourceIndex");
        writer.Int(entityTypeResourceIndex);

        writer.String("propertyValues");
        propertyValues.SerializeToJSON(writer);

        writer.String("postInitPropertyValues");
        postInitPropertyValues.SerializeToJSON(writer);

        writer.EndObject();
    }

    void STemplateSubEntity::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int parentIndexOffset = offset + offsetof(STemplateSubEntity, parentIndex);
        unsigned int entityTypeResourceIndexOffset = offset + offsetof(STemplateSubEntity, entityTypeResourceIndex);
        unsigned int propertyValuesOffset = offset + offsetof(STemplateSubEntity, propertyValues);
        unsigned int postInitPropertyValuesOffset = offset + offsetof(STemplateSubEntity, postInitPropertyValues);

        binarySerializer.WriteToMemory(&parentIndex, sizeof(int), parentIndexOffset);
        binarySerializer.WriteToMemory(&entityTypeResourceIndex, sizeof(int), entityTypeResourceIndexOffset);

        propertyValues.SerializeToMemory(binarySerializer, propertyValuesOffset);
        postInitPropertyValues.SerializeToMemory(binarySerializer, postInitPropertyValuesOffset);
    }

    STemplateSubEntity* STemplateSubEntity::DeserializeFromJSON(const rapidjson::Value& object)
    {
        STemplateSubEntity* templateSubEntity = new STemplateSubEntity();
        const Value& propertyValues = object["propertyValues"];
        const Value& postInitPropertyValues = object["propertyValues"];

        templateSubEntity->parentIndex = object["parentIndex"].GetInt();
        templateSubEntity->entityTypeResourceIndex = object["entityTypeResourceIndex"].GetInt();
        templateSubEntity->propertyValues = *TArray<SEntityTemplateProperty>::DeserializeFromJSON(object["propertyValues"].GetArray());
        templateSubEntity->postInitPropertyValues = *TArray<SEntityTemplateProperty>::DeserializeFromJSON(object["postInitPropertyValues"].GetArray());

        return templateSubEntity;
    }
}
