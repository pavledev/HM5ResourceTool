#include "pch.h"
#include "Glacier2/Entity/SEntityTemplateReference.hpp"
#include "Glacier2/ZBinarySerializer.hpp"

namespace IOI
{
    void SEntityTemplateReference::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("entityIndex");
        writer.Int(entityIndex);
        writer.String("exposedEntity");
        writer.String(exposedEntity.ToCString());

        writer.EndObject();
    }

    void SEntityTemplateReference::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int entityIndexOffset = offset + offsetof(SEntityTemplateReference, entityIndex);
        unsigned int exposedEntityOffset = offset + offsetof(SEntityTemplateReference, exposedEntity);

        binarySerializer.WriteToMemory(&entityIndex, sizeof(int), entityIndexOffset);

        exposedEntity.SerializeToMemory(binarySerializer, exposedEntityOffset);
    }

    SEntityTemplateReference* SEntityTemplateReference::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SEntityTemplateReference* entityTemplateReference = new SEntityTemplateReference();

        entityTemplateReference->entityIndex = object["entityIndex"].GetInt();
        entityTemplateReference->exposedEntity = *ZString::DeserializeFromJSON(object["exposedEntity"].GetString());

        return entityTemplateReference;
    }

    bool SEntityTemplateReference::operator==(const SEntityTemplateReference& other)
    {
        return entityIndex == other.entityIndex && exposedEntity == other.exposedEntity;
    }
}
