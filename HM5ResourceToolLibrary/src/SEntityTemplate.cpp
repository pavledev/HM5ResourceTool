#include "pch.h"
#include <format>
#include "SEntityTemplate.hpp"
#include "ZUtility.hpp"
#include "Glacier2/Math/SVector2.hpp"
#include "Glacier2/Math/SVector3.hpp"
#include "Glacier2/Math/SMatrix43.hpp"
#include "Glacier2/SColorRGB.hpp"
#include "Glacier2/SColorRGBA.hpp"
#include "Glacier2/Entity/SEntityTemplateReference.hpp"
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"
#include "Glacier2/TArray.hpp"
#include "Glacier2/ZCurve.hpp"
#include "Glacier2/ZGameTime.hpp"
#include "Glacier2/ZSharedSensorDef.hpp"
#include "Glacier2/Sound/ZSpeakerLevels.hpp"

namespace HM5ResourceTool
{
    void SEntityTemplate::SSubEntityTemplate::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.String("templateEntity");
        writer.String(templateEntityResourceID.c_str());

        writer.String("templateEntityBlueprint");
        writer.String(templateEntityBlueprintResourceID.c_str());

        writer.String("parentIndex");
        writer.Int(parentIndex);

        writer.String("entityTypeResourceIndex");
        writer.Int(entityTypeResourceIndex);

        writer.String("entityName");
        writer.String(entityName.ToCString());

        writer.String("propertyValues");
        propertyValues.SerializeToJSON(writer);

        writer.String("postInitPropertyValues");
        postInitPropertyValues.SerializeToJSON(writer);

        writer.String("propertyAliases");
        propertyAliases.SerializeToJSON(writer);

        writer.String("exposedEntities");
        exposedEntities.SerializeToJSON(writer);

        writer.String("exposedInterfaces");
        exposedInterfaces.SerializeToJSON(writer);

        writer.String("entitySubsets");
        entitySubsets.SerializeToJSON(writer);
    }

    std::map<std::string, std::map<std::string, std::vector<std::string>>> SEntityTemplate::SSubEntityTemplate::GetPropertyTypesAndValues()
    {
        std::map<std::string, std::map<std::string, std::vector<std::string>>> result;

        for (unsigned int j = 0; j < propertyValues.Size(); ++j)
        {
            std::string propertyName = propertyRegistry.GetPropertyNameFromHash(propertyValues[j].nPropertyID);

            result.insert(std::make_pair(propertyName, GetPropertyTypeAndValues(&propertyValues[j])));
        }

        return result;
    }

    std::map<std::string, std::map<std::string, std::vector<std::string>>> SEntityTemplate::SSubEntityTemplate::GetPostInitPropertyTypeAndValues()
    {
        std::map<std::string, std::map<std::string, std::vector<std::string>>> result;

        for (unsigned int j = 0; j < postInitPropertyValues.Size(); ++j)
        {
            std::string propertyName = propertyRegistry.GetPropertyNameFromHash(postInitPropertyValues[j].nPropertyID);

            result.insert(std::make_pair(propertyName, GetPropertyTypeAndValues(&postInitPropertyValues[j])));
        }

        return result;
    }

    std::map<std::string, std::vector<std::string>> SEntityTemplate::SSubEntityTemplate::GetPropertyTypeAndValues(IOI::SEntityTemplateProperty* entityTemplateProperty)
    {
        std::map<std::string, std::vector<std::string>> result;
        std::vector<std::string> values;
        std::string typeName = entityTemplateProperty->value.GetTypeID()->pTypeInfo->pszTypeName;
        void* data = entityTemplateProperty->value.GetData();

        if (typeName == "bool")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<bool*>(data)));
        }
        else if (typeName == "int8" || typeName == "char")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<char*>(data)));
        }
        else if (typeName == "uint8")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<unsigned char*>(data)));
        }
        else if (typeName == "int16")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<short*>(data)));
        }
        else if (typeName == "uint16")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<unsigned short*>(data)));
        }
        else if (typeName == "int32")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<int*>(data)));
        }
        else if (typeName == "uint32")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<unsigned int*>(data)));
        }
        else if (typeName == "int64")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<long long*>(data)));
        }
        else if (typeName == "uint64")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<unsigned long long*>(data)));
        }
        else if (typeName == "float32")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<float*>(data)));
        }
        else if (typeName == "float64")
        {
            values.push_back(ZUtility::ConvertValueToString(*static_cast<double*>(data)));
        }
        else if (typeName == "SVector2")
        {
            IOI::SVector2* vector = static_cast<IOI::SVector2*>(data);

            values.push_back(std::format("{}|{}", vector->x, vector->y));
        }
        else if (typeName == "SVector3")
        {
            IOI::SVector3* vector = static_cast<IOI::SVector3*>(data);

            values.push_back(std::format("{}|{}|{}", vector->x, vector->y, vector->z));
        }
        else if (typeName == "SMatrix43")
        {
            IOI::SMatrix43* matrix = static_cast<IOI::SMatrix43*>(data);
            IOI::SVector3 rotation = matrix->Rot.ConvertRotationMatrixToEulerAngles();

            values.push_back(std::format("{}|{}|{}#{}|{}|{}",
                rotation.x, rotation.y, rotation.z,
                matrix->Trans.x, matrix->Trans.y, matrix->Trans.z));
        }
        else if (typeName == "SColorRGB")
        {
            IOI::SColorRGB* colorRGB = static_cast<IOI::SColorRGB*>(data);

            values.push_back(std::format("{}|{}|{}", colorRGB->r, colorRGB->g, colorRGB->b));
        }
        else if (typeName == "SColorRGBA")
        {
            IOI::SColorRGBA* colorRGBA = static_cast<IOI::SColorRGBA*>(data);

            values.push_back(std::format("{}|{}|{}|{}", colorRGBA->r, colorRGBA->g, colorRGBA->b, colorRGBA->a));
        }
        else if (typeName == "SEntityTemplateReference")
        {
            IOI::SEntityTemplateReference* entityTemplateReference = static_cast<IOI::SEntityTemplateReference*>(data);

            values.push_back(std::format("{}|{}", entityTemplateReference->entityIndex, entityTemplateReference->exposedEntity.ToCString()));
        }
        else if (typeName == "ZRuntimeResourceID")
        {
            IOI::ZRuntimeResourceID* runtimeResourceID = static_cast<IOI::ZRuntimeResourceID*>(data);
            //Add resource id instead of hash and do same for json

            //values.push_back(std::format("{}|{}", runtimeResourceID->GetHigh(), runtimeResourceID->GetLow()));
            values.push_back(std::to_string(runtimeResourceID->GetID()));
        }
        else if (typeName == "ZString")
        {
            IOI::ZString* string = static_cast<IOI::ZString*>(data);

            values.push_back(string->ToCString());
        }
        else if (typeName == "ZCurve")
        {
            IOI::ZCurve* curve = static_cast<IOI::ZCurve*>(data);

            for (unsigned int i = 0; i < curve->data.Size(); i++)
            {
                IOI::TFixedArray<float, 8>* array = &curve->data[i];
                std::string data2;

                for (unsigned j = 0; j < array->Size(); ++j)
                {
                    data2 += ZUtility::ConvertValueToString(array->operator[](j));

                    if (j < array->Size() - 1)
                    {
                        data2 += "|";
                    }
                }

                values.push_back(data2);
            }
        }
        else if (typeName == "ZGameTime")
        {
            IOI::ZGameTime* gameTime = static_cast<IOI::ZGameTime*>(data);

            values.push_back(ZUtility::ConvertValueToString(gameTime->GetTicks()));
        }
        else if (typeName == "ZSpeakerLevels")
        {
            IOI::ZSpeakerLevels* speakerLevels = static_cast<IOI::ZSpeakerLevels*>(data);
            IOI::TArray<float> speakerMatrix = speakerLevels->GetSpeakerMatrix();

            for (unsigned int i = 0; i < speakerMatrix.Size(); i++)
            {
                values.push_back(ZUtility::ConvertValueToString(speakerMatrix[i]));
            }
        }
        else if (typeName == "TArray<SEntityTemplateReference>")
        {
            IOI::TArray<IOI::SEntityTemplateReference>* entityTemplateReferences = static_cast<IOI::TArray<IOI::SEntityTemplateReference>*>(data);

            for (unsigned int i = 0; i < entityTemplateReferences->Size(); i++)
            {
                IOI::SEntityTemplateReference* entityTemplateReference = &entityTemplateReferences->operator[](i);

                values.push_back(std::format("{}|{}", entityTemplateReference->entityIndex, entityTemplateReference->exposedEntity.ToCString()));
            }
        }
        else if (typeName == "TArray<float32>")
        {
            IOI::TArray<float>* array = static_cast<IOI::TArray<float>*>(data);

            for (unsigned int i = 0; i < array->Size(); i++)
            {
                values.push_back(ZUtility::ConvertValueToString(array->operator[](i)));
            }
        }
        else if (typeName == "TArray<ZGameTime>")
        {
            IOI::TArray<IOI::ZGameTime>* array = static_cast<IOI::TArray<IOI::ZGameTime>*>(data);

            for (unsigned int i = 0; i < array->Size(); i++)
            {
                values.push_back(ZUtility::ConvertValueToString(array->operator[](i).GetTicks()));
            }
        }
        else if (typeName == "TArray<SVector2>")
        {
            IOI::TArray<IOI::SVector2>* array = static_cast<IOI::TArray<IOI::SVector2>*>(data);

            for (unsigned int i = 0; i < array->Size(); i++)
            {
                IOI::SVector2* vector = &array->operator[](i);

                values.push_back(std::format("{}|{}", vector->x, vector->y));
            }
        }
        else if (typeName == "TArray<bool>")
        {
            IOI::TArray<bool>* array = static_cast<IOI::TArray<bool>*>(data);

            for (unsigned int i = 0; i < array->Size(); i++)
            {
                values.push_back(ZUtility::ConvertValueToString(array->operator[](i)));
            }
        }
        else if (typeName == "TArray<ZSharedSensorDef.SVisibilitySetting>")
        {
            /*IOI::TArray<IOI::ZSharedSensorDef::SVisibilitySetting>* array = static_cast<IOI::TArray<IOI::ZSharedSensorDef::SVisibilitySetting>*>(data);

            for (unsigned int i = 0; i < array->Size(); i++)
            {
                data2 += ZUtility::ConvertValueToString(array->operator[](i));

                if (i < array->Size() - 1)
                {
                    data2 += "|";
                }
            }

            values.push_back(data2);*/
        }
        else if (typeName == "TArray<ZString>")
        {
            IOI::TArray<IOI::ZString>* array = static_cast<IOI::TArray<IOI::ZString>*>(data);

            for (unsigned int i = 0; i < array->Size(); i++)
            {
                values.push_back(array->operator[](i).ToCString());
            }
        }
        else if (entityTemplateProperty->value.GetTypeID()->pTypeInfo->IsEnumType())
        {
            if (enumRegistry.IsEmpty())
            {
                LoadEnumRegistry();
            }

            std::map<std::string, int> items = enumRegistry.GetEnumByTypeName(typeName);

            int value = *static_cast<int*>(data);

            for (auto it = items.begin(); it != items.end(); it++)
            {
                if (it->second == value)
                {
                    values.push_back(it->first.c_str());

                    break;
                }
            }

            typeName.insert(0, "Enum#");
        }

        result.insert(std::make_pair(typeName, values));

        return result;
    }

    std::vector<std::string> SEntityTemplate::SSubEntityTemplate::GetPropertyAliases()
    {
        std::vector<std::string> result;

        for (unsigned int i = 0; i < propertyAliases.Size(); ++i)
        {
            result.push_back(std::format("{}|{}|{}", propertyAliases[i].sAliasName.ToCString(), propertyAliases[i].entityID, propertyAliases[i].sPropertyName.ToCString()));
        }

        return result;
    }

    SEntityTemplate::SEntityTemplate()
    {
        blueprintIndexInResourceHeader = 0;
        rootEntityIndex = 0;
        rootEntityTemplate = nullptr;

        if (propertyRegistry.IsEmpty())
        {
            char buffer[MAX_PATH];

            GetModuleFileNameA(NULL, buffer, MAX_PATH);

            std::string::size_type pos = std::string(buffer).find_last_of("\\/");
            std::string filePath = std::format("{}/Properties.json", std::string(buffer).substr(0, pos));
            HM5ResourceTool::IO::ZJSONDeserializer jsonDeserializer = HM5ResourceTool::IO::ZJSONDeserializer(filePath);

            jsonDeserializer.Deserialize<ZPropertyRegistry>(propertyRegistry);
        }
    }

    std::string SEntityTemplate::GetEntityDetails(int index)
    {
        SEntityTemplate::SSubEntityTemplate* entity = FindEntity(index);

        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        entity->SerializeToJSON(writer);

        writer.String("pinConnections");
        SerializePinsToJSON(entity, pinConnections, writer);

        writer.String("inputPinForwardings");
        SerializePinsToJSON(entity, inputPinForwardings, writer);

        writer.String("outputPinForwardings");
        SerializePinsToJSON(entity, outputPinForwardings, writer);

        writer.EndObject();

        return stringBuffer.GetString();
    }

    std::string SEntityTemplate::GetEntityName(int index)
    {
        SEntityTemplate::SSubEntityTemplate* entity = FindEntity(index);

        return entity->entityName.ToCString();
    }

    std::string SEntityTemplate::GetTemplateEntityResourceID(int index)
    {
        SEntityTemplate::SSubEntityTemplate* entity = FindEntity(index);

        return entity->templateEntityResourceID;
    }

    std::string SEntityTemplate::GetTemplateEntityBlueprintResourceID(int index)
    {
        SEntityTemplate::SSubEntityTemplate* entity = FindEntity(index);

        return entity->templateEntityBlueprintResourceID;
    }

    SEntityTemplate::SSubEntityTemplate* SEntityTemplate::FindEntity(int index)
    {
        SEntityTemplate::SSubEntityTemplate* entity = nullptr;

        if (rootEntityTemplate->index == index || rootEntityTemplate->children.Size() == 0)
        {
            entity = rootEntityTemplate;
        }
        else
        {
            entity = FindEntity(rootEntityTemplate, index);
        }

        return entity;
    }

    SEntityTemplate::SSubEntityTemplate* SEntityTemplate::FindEntity(SEntityTemplate::SSubEntityTemplate* subEntityTemplate, int index)
    {
        SEntityTemplate::SSubEntityTemplate* result = nullptr;

        for (unsigned int i = 0; i < subEntityTemplate->children.Size(); ++i)
        {
            if (subEntityTemplate->children[i]->index == index)
            {
                result = subEntityTemplate->children[i];

                break;
            }

            result = FindEntity(subEntityTemplate->children[i], index);

            if (result)
            {
                break;
            }
        }

        return result;
    }

    void SEntityTemplate::SerializePinsToJSON(SEntityTemplate::SSubEntityTemplate* entity, const IOI::TArray<IOI::SEntityTemplatePinConnection>& pins, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartArray();

        for (unsigned int i = 0; i < pins.Size(); ++i)
        {
            if (pins[i].fromID == entity->index || pins[i].toID == entity->index)
            {
                writer.StartObject();

                writer.String("fromEntity");

                SEntityTemplate::SSubEntityTemplate* entity2 = nullptr;

                if (pins[i].fromID == entity->index)
                {
                    entity2 = FindEntity(pinConnections[i].toID);

                    writer.String(entity->entityName.ToCString());
                }
                else
                {
                    entity2 = FindEntity(pinConnections[i].fromID);

                    writer.String(entity2->entityName.ToCString());
                }

                writer.String("fromPinName");
                writer.String(pinConnections[i].fromPinName.ToCString());

                writer.String("toEntity");

                if (pinConnections[i].fromID == entity->index)
                {
                    writer.String(entity2->entityName.ToCString());
                }
                else
                {
                    writer.String(entity->entityName.ToCString());
                }

                writer.String("toPinName");
                writer.String(pinConnections[i].toPinName.ToCString());

                writer.EndObject();
            }
        }

        writer.EndArray();
    }

    std::set<int> SEntityTemplate::SearchContent(const std::string& content, const bool exactMatch)
    {
        std::set<int> result;

        SearchContent(rootEntityTemplate, content, exactMatch, result);

        return result;
    }

    void SEntityTemplate::SearchContent(SEntityTemplate::SSubEntityTemplate* subEntityTemplate, const std::string& content, const bool exactMatch, std::set<int>& indices)
    {
        for (unsigned int i = 0; i < subEntityTemplate->children.Size(); ++i)
        {
            SearchContent(subEntityTemplate->children[i], content, exactMatch, indices);

            bool found = SearchContent(subEntityTemplate->children[i], content, exactMatch);

            if (found)
            {
                indices.insert(subEntityTemplate->children[i]->index);
                indices.insert(subEntityTemplate->children[i]->parentIndex);
            }
            else
            {
                for (unsigned j = 0; j < subEntityTemplate->children.Size(); ++j)
                {
                    if (indices.contains(subEntityTemplate->children[i]->index))
                    {
                        indices.insert(subEntityTemplate->index);

                        break;
                    }
                }
            }
        }
    }

    bool SEntityTemplate::SearchContent(SEntityTemplate::SSubEntityTemplate* subEntityTemplate, const std::string& content, const bool exactMatch)
    {
        bool found = false;

        if (exactMatch)
        {
            if (content == subEntityTemplate->entityName.ToCString())
            {
                found = true;
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->propertyValues.Size(); ++j)
                {
                    std::string propertyName = propertyRegistry.GetPropertyNameFromHash(subEntityTemplate->propertyValues[j].nPropertyID);

                    if (content == propertyName)
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->postInitPropertyValues.Size(); ++j)
                {
                    std::string propertyName = propertyRegistry.GetPropertyNameFromHash(subEntityTemplate->postInitPropertyValues[j].nPropertyID);

                    if (content == propertyName)
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->propertyAliases.Size(); ++j)
                {
                    if (content == subEntityTemplate->propertyAliases[j].sPropertyName.ToCString() ||
                        content == subEntityTemplate->propertyAliases[j].sAliasName.ToCString())
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->exposedEntities.Size(); ++j)
                {
                    if (content == subEntityTemplate->exposedEntities[j].m_key.ToCString())
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->exposedInterfaces.Size(); ++j)
                {
                    if (content == subEntityTemplate->exposedInterfaces[j].m_key.ToCString())
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->entitySubsets.Size(); ++j)
                {
                    if (content == subEntityTemplate->entitySubsets[j].m_key.ToCString())
                    {
                        found = true;

                        break;
                    }
                }
            }
        }
        else
        {
            if (content.find(subEntityTemplate->entityName.ToCString()) != std::string::npos)
            {
                found = true;
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->propertyValues.Size(); ++j)
                {
                    std::string propertyName = propertyRegistry.GetPropertyNameFromHash(subEntityTemplate->propertyValues[j].nPropertyID);

                    if (content.find(propertyName) != std::string::npos)
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->propertyAliases.Size(); ++j)
                {
                    if (content.find(subEntityTemplate->propertyAliases[j].sPropertyName.ToCString()) != std::string::npos ||
                        content.find(subEntityTemplate->propertyAliases[j].sAliasName.ToCString()) != std::string::npos)
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->exposedEntities.Size(); ++j)
                {
                    if (content.find(subEntityTemplate->exposedEntities[j].m_key.ToCString()) != std::string::npos)
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->exposedInterfaces.Size(); ++j)
                {
                    if (content.find(subEntityTemplate->exposedInterfaces[j].m_key.ToCString()) != std::string::npos)
                    {
                        found = true;

                        break;
                    }
                }
            }

            if (!found)
            {
                for (unsigned int j = 0; j < subEntityTemplate->entitySubsets.Size(); ++j)
                {
                    if (content.find(subEntityTemplate->entitySubsets[j].m_key.ToCString()) != std::string::npos)
                    {
                        found = true;

                        break;
                    }
                }
            }
        }

        return found;
    }

    void SEntityTemplate::LoadEnumRegistry()
    {
        char buffer[MAX_PATH];

        GetModuleFileNameA(NULL, buffer, MAX_PATH);

        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        std::string filePath = std::format("{}/Enums.json", std::string(buffer).substr(0, pos));
        HM5ResourceTool::IO::ZJSONDeserializer jsonDeserializer = HM5ResourceTool::IO::ZJSONDeserializer(filePath);

        jsonDeserializer.Deserialize<ZEnumRegistry>(enumRegistry);
    }

    std::map<std::string, std::map<std::string, std::vector<std::string>>> SEntityTemplate::GetPropertyTypesAndValues(int index)
    {
        SEntityTemplate::SSubEntityTemplate* entity = FindEntity(index);

        return entity->GetPropertyTypesAndValues();
    }

    std::map<std::string, std::map<std::string, std::vector<std::string>>> SEntityTemplate::GetPostInitPropertyTypesAndValues(int index)
    {
        SEntityTemplate::SSubEntityTemplate* entity = FindEntity(index);

        return entity->GetPostInitPropertyTypeAndValues();
    }

    std::vector<std::string> SEntityTemplate::GetEntityReferences(int index)
    {
        std::vector<std::string> references;
        SEntityTemplate::SSubEntityTemplate* entity = FindEntity(index);
        SEntityTemplate::SSubEntityTemplate* parentEntity = nullptr;

        if (entity->parentIndex != -1)
        {
            parentEntity = FindEntity(entity->parentIndex);

            references.push_back(std::format("Parent: {}", parentEntity->entityName.ToCString()));
        }

        GetPropertyReferences(&entity->propertyValues, references);
        GetPropertyReferences(&entity->postInitPropertyValues, references);
        GetPinReferences(index, references);

        return references;
    }

    void SEntityTemplate::GetPropertyReferences(IOI::TArray<IOI::SEntityTemplateProperty>* properties, std::vector<std::string>& references)
    {
        for (unsigned int i = 0; i < properties->Size(); ++i)
        {
            std::string propertyName = propertyRegistry.GetPropertyNameFromHash(properties->operator[](i).nPropertyID);
            std::string typeName = properties->operator[](i).value.GetTypeID()->pTypeInfo->pszTypeName;

            if (typeName == "SEntityTemplateReference")
            {
                IOI::SEntityTemplateReference* entityTemplateReference = static_cast<IOI::SEntityTemplateReference*>(properties->operator[](i).value.GetData());
                std::string entityName = GetEntityName(entityTemplateReference->entityIndex);

                references.push_back(std::format("Property: {} ({})", propertyName, entityName));
            }
            else if (typeName == "TArray<SEntityTemplateReference>")
            {
                IOI::TArray<IOI::SEntityTemplateReference>* entityTemplateReferences = static_cast<IOI::TArray<IOI::SEntityTemplateReference>*>(properties->operator[](i).value.GetData());
                
                if (entityTemplateReferences->Size() > 0)
                {
                    std::string propertyReferences = std::format("Property: {} (", propertyName);

                    for (unsigned int j = 0; j < entityTemplateReferences->Size(); ++j)
                    {
                        std::string entityName = GetEntityName(entityTemplateReferences->operator[](j).entityIndex);

                        propertyReferences += entityName;

                        if (j < entityTemplateReferences->Size() - 1)
                        {
                            propertyReferences += ", ";
                        }
                    }

                    propertyReferences += ")";

                    references.push_back(propertyReferences);
                }
            }
        }
    }

    void SEntityTemplate::GetPinReferences(int entityIndex, std::vector<std::string>& references)
    {
        std::string pinReferences = "Pins: ";

        for (unsigned int i = 0; i < pinConnections.Size(); ++i)
        {
            if (pinConnections[i].fromID == entityIndex)
            {
                SEntityTemplate::SSubEntityTemplate* entity = FindEntity(pinConnections[i].toID);

                pinReferences += std::format("{} ({})", pinConnections[i].toPinName.ToCString(), entity->entityName.ToCString());

                if (i < pinConnections.Size() - 1)
                {
                    pinReferences += ", ";
                }
            }
        }

        if (pinReferences.length() > 6)
        {
            references.push_back(pinReferences);
        }
    }

    std::set<std::string> SEntityTemplate::GetEntityBackReferences(int index)
    {
        std::set<std::string> backReferences;

        SEntityTemplate::SSubEntityTemplate* entity = nullptr;

        if (rootEntityTemplate->index == index || rootEntityTemplate->children.Size() == 0)
        {
            entity = rootEntityTemplate;

            GetPropertyBackReferences(index, &entity->propertyValues, backReferences);
            GetPropertyBackReferences(index, &entity->postInitPropertyValues, backReferences);
            GetPinBackReferences(index, backReferences);
        }
        else
        {
            GetEntityBackReferences(index, rootEntityTemplate, backReferences);
        }

        return backReferences;
    }

    void SEntityTemplate::GetEntityBackReferences(int index, SEntityTemplate::SSubEntityTemplate* entity, std::set<std::string>& backReferences)
    {
        for (unsigned int i = 0; i < entity->children.Size(); ++i)
        {
            if (entity->children[i]->index == index)
            {
                continue;
            }

            GetPropertyBackReferences(index, &entity->children[i]->propertyValues, backReferences);
            GetPropertyBackReferences(index, &entity->children[i]->postInitPropertyValues, backReferences);
            GetPinBackReferences(index, backReferences);

            GetEntityBackReferences(index, entity->children[i], backReferences);
        }
    }

    void SEntityTemplate::GetPropertyBackReferences(int index, IOI::TArray<IOI::SEntityTemplateProperty>* properties, std::set<std::string>& backReferences)
    {
        for (unsigned int i = 0; i < properties->Size(); ++i)
        {
            std::string propertyName = propertyRegistry.GetPropertyNameFromHash(properties->operator[](i).nPropertyID);
            std::string typeName = properties->operator[](i).value.GetTypeID()->pTypeInfo->pszTypeName;

            if (typeName == "SEntityTemplateReference")
            {
                IOI::SEntityTemplateReference* entityTemplateReference = static_cast<IOI::SEntityTemplateReference*>(properties->operator[](i).value.GetData());

                if (entityTemplateReference->entityIndex == index)
                {
                    std::string entityName = GetEntityName(entityTemplateReference->entityIndex);

                    backReferences.insert(std::format("Property: {} ({})", propertyName, entityName));
                }
            }
            else if (typeName == "TArray<SEntityTemplateReference>")
            {
                IOI::TArray<IOI::SEntityTemplateReference>* entityTemplateReferences = static_cast<IOI::TArray<IOI::SEntityTemplateReference>*>(properties->operator[](i).value.GetData());

                if (entityTemplateReferences->Size() > 0)
                {
                    std::string propertyReferences = std::format("Property: {} (", propertyName);

                    for (unsigned int j = 0; j < entityTemplateReferences->Size(); ++j)
                    {
                        int entityIndex = entityTemplateReferences->operator[](j).entityIndex;

                        if (entityIndex == index)
                        {
                            std::string entityName = GetEntityName(entityTemplateReferences->operator[](j).entityIndex);

                            propertyReferences += entityName;

                            if (j < entityTemplateReferences->Size() - 1)
                            {
                                propertyReferences += ", ";
                            }
                        }
                    }

                    if (!propertyReferences.ends_with("("))
                    {
                        if (propertyReferences.ends_with(", "))
                        {
                            propertyReferences.erase(propertyReferences.length() - 2, 2);
                        }

                        propertyReferences += ")";

                        backReferences.insert(propertyReferences);
                    }
                }
            }
        }
    }

    void SEntityTemplate::GetPinBackReferences(int entityIndex, std::set<std::string>& backReferences)
    {
        std::string pinReferences = "Pins: ";

        for (unsigned int i = 0; i < pinConnections.Size(); ++i)
        {
            if (pinConnections[i].toID == entityIndex)
            {
                SEntityTemplate::SSubEntityTemplate* entity = FindEntity(pinConnections[i].fromID);

                pinReferences += std::format("{} ({})", pinConnections[i].fromPinName.ToCString(), entity->entityName.ToCString());

                if (i < pinConnections.Size() - 1)
                {
                    pinReferences += ", ";
                }
            }
        }

        if (pinReferences.length() > 6)
        {
            if (pinReferences.ends_with(", "))
            {
                pinReferences.erase(pinReferences.length() - 2, 2);
            }

            backReferences.insert(pinReferences);
        }
    }

    std::vector<std::string> SEntityTemplate::GetPropertyAliases(int index)
    {
        SEntityTemplate::SSubEntityTemplate* entity = FindEntity(index);

        return entity->GetPropertyAliases();
    }
    std::map<std::string, std::string> SEntityTemplate::GetExposedEntities(int index)
    {
        return std::map<std::string, std::string>();
    }
}
