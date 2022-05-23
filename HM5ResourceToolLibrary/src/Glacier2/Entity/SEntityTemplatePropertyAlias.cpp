#include "pch.h"
#include "Glacier2/Entity/SEntityTemplatePropertyAlias.hpp"

namespace IOI
{
    void SEntityTemplatePropertyAlias::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("sAliasName");
        sAliasName.SerializeToJSON(writer);

        writer.String("entityID");
        writer.Int(entityID);

        writer.String("sPropertyName");
        sPropertyName.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SEntityTemplatePropertyAlias::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int sAliasNameOffset = offset + offsetof(SEntityTemplatePropertyAlias, sAliasName);
        unsigned int entityIDOffset = offset + offsetof(SEntityTemplatePropertyAlias, entityID);
        unsigned int sPropertyNameOffset = offset + offsetof(SEntityTemplatePropertyAlias, sPropertyName);

        sAliasName.SerializeToMemory(binarySerializer, sAliasNameOffset);

        binarySerializer.WriteToMemory(&entityID, sizeof(int), entityIDOffset);

        sPropertyName.SerializeToMemory(binarySerializer, sPropertyNameOffset);
    }

    SEntityTemplatePropertyAlias* SEntityTemplatePropertyAlias::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SEntityTemplatePropertyAlias* entityTemplatePropertyAlias = new SEntityTemplatePropertyAlias();

        entityTemplatePropertyAlias->sAliasName = *ZString::DeserializeFromJSON(object["sAliasName"].GetString());
        entityTemplatePropertyAlias->entityID = object["entityID"].GetInt();
        entityTemplatePropertyAlias->sPropertyName = ZString(object["entityTemplates"].GetString());

        return entityTemplatePropertyAlias;
    }
}
