#include "pch.h"
#include "Glacier2/Entity/SEntityTemplateEntitySubset.hpp"

namespace IOI
{
    void SEntityTemplateEntitySubset::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("subsetType");

        std::string typeName = subsetType->pTypeInfo->pszTypeName;

        writer.String(typeName.c_str());

        writer.String("entities");
        entities.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SEntityTemplateEntitySubset::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int subsetTypeOffset = offset + offsetof(SEntityTemplateEntitySubset, subsetType);
        unsigned int entitiesOffset = offset + offsetof(SEntityTemplateEntitySubset, entities);

        subsetType->SerializeToMemory(binarySerializer, subsetTypeOffset);
        entities.SerializeToMemory(binarySerializer, entitiesOffset);
    }

    SEntityTemplateEntitySubset* SEntityTemplateEntitySubset::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SEntityTemplateEntitySubset* entityTemplateEntitySubset = new SEntityTemplateEntitySubset();

        entityTemplateEntitySubset->subsetType = STypeID::DeserializeFromJSON(object, "subsetType");
        entityTemplateEntitySubset->entities = *TArray<int>::DeserializeFromJSON(object["entities"].GetArray());

        return entityTemplateEntitySubset;
    }
}
