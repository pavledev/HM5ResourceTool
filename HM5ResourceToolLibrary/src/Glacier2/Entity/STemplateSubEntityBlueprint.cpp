#include "pch.h"
#include "Glacier2/Entity/STemplateSubEntityBlueprint.hpp"

namespace IOI
{
    void STemplateSubEntityBlueprint::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("parentIndex");
        writer.Int(parentIndex);

        writer.String("entityTypeResourceIndex");
        writer.Int(entityTypeResourceIndex);

        writer.String("entityName");
        entityName.SerializeToJSON(writer);

        writer.String("propertyAliases");
        propertyAliases.SerializeToJSON(writer);

        writer.String("exposedEntities");
        exposedEntities.SerializeToJSON(writer);

        writer.String("exposedInterfaces");
        exposedInterfaces.SerializeToJSON(writer);

        writer.String("entitySubsets");
        entitySubsets.SerializeToJSON(writer);

        writer.EndObject();
    }

    void STemplateSubEntityBlueprint::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int parentIndexOffset = offset + offsetof(STemplateSubEntityBlueprint, parentIndex);
        unsigned int entityTypeResourceIndexOffset = offset + offsetof(STemplateSubEntityBlueprint, entityTypeResourceIndex);
        unsigned int entityNameOffset = offset + offsetof(STemplateSubEntityBlueprint, entityName);
        unsigned int propertyAliasesOffset = offset + offsetof(STemplateSubEntityBlueprint, propertyAliases);
        unsigned int exposedEntitiesOffset = offset + offsetof(STemplateSubEntityBlueprint, exposedEntities);
        unsigned int exposedInterfacesOffset = offset + offsetof(STemplateSubEntityBlueprint, exposedInterfaces);
        unsigned int entitySubsetsOffset = offset + offsetof(STemplateSubEntityBlueprint, entitySubsets);

        binarySerializer.WriteToMemory(&parentIndex, sizeof(int), parentIndexOffset);
        binarySerializer.WriteToMemory(&entityTypeResourceIndex, sizeof(int), entityTypeResourceIndexOffset);

        entityName.SerializeToMemory(binarySerializer, entityNameOffset);
        propertyAliases.SerializeToMemory(binarySerializer, propertyAliasesOffset);
        exposedEntities.SerializeToMemory(binarySerializer, exposedEntitiesOffset);
        exposedInterfaces.SerializeToMemory(binarySerializer, exposedInterfacesOffset);
        entitySubsets.SerializeToMemory(binarySerializer, entitySubsetsOffset);
    }

    STemplateSubEntityBlueprint* STemplateSubEntityBlueprint::DeserializeFromJSON(const rapidjson::Value& object)
    {
        STemplateSubEntityBlueprint* templateSubEntityBlueprint = new STemplateSubEntityBlueprint();

        templateSubEntityBlueprint->parentIndex = object["parentIndex"].GetInt();
        templateSubEntityBlueprint->entityTypeResourceIndex = object["entityTypeResourceIndex"].GetInt();
        templateSubEntityBlueprint->entityName = *ZString::DeserializeFromJSON(object["entityName"].GetString());
        templateSubEntityBlueprint->propertyAliases = *TArray<SEntityTemplatePropertyAlias>::DeserializeFromJSON(object["propertyAliases"].GetArray());
        templateSubEntityBlueprint->exposedEntities = *TArray<TPair<ZString, SEntityTemplateReference>>::DeserializeFromJSON(object["exposedEntities"].GetArray());
        templateSubEntityBlueprint->exposedInterfaces = *TArray<TPair<ZString, int>>::DeserializeFromJSON(object["exposedInterfaces"].GetArray());
        templateSubEntityBlueprint->entitySubsets = *TArray<TPair<ZString, SEntityTemplateEntitySubset>>::DeserializeFromJSON(object["entitySubsets"].GetArray());

        return templateSubEntityBlueprint;
    }
}
