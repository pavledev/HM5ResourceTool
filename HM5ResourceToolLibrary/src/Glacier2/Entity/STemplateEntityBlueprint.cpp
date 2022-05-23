#include "pch.h"
#include "Glacier2/Entity/STemplateEntityBlueprint.hpp"

namespace IOI
{
    std::string STemplateEntityBlueprint::SerializeToJSON()
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        writer.String("rootEntityIndex");
        writer.Int(rootEntityIndex);

        writer.String("entityTemplates");
        entityTemplates.SerializeToJSON(writer);

        writer.String("pinConnections");
        pinConnections.SerializeToJSON(writer);

        writer.String("inputPinForwardings");
        inputPinForwardings.SerializeToJSON(writer);

        writer.String("outputPinForwardings");
        outputPinForwardings.SerializeToJSON(writer);

        writer.EndObject();

        return stringBuffer.GetString();
    }

    void STemplateEntityBlueprint::SerializeToMemory(ZBinarySerializer& binarySerializer)
    {
        unsigned int rootEntityIndexOffset = offsetof(STemplateEntityBlueprint, rootEntityIndex);
        unsigned int entityTemplatesOffset = offsetof(STemplateEntityBlueprint, entityTemplates);
        unsigned int pinConnectionsOffset = offsetof(STemplateEntityBlueprint, pinConnections);
        unsigned int inputPinForwardingsOffset = offsetof(STemplateEntityBlueprint, inputPinForwardings);
        unsigned int outputPinForwardingsOffset = offsetof(STemplateEntityBlueprint, outputPinForwardings);

        binarySerializer.WriteToMemory(&rootEntityIndex, sizeof(int), rootEntityIndexOffset);

        entityTemplates.SerializeToMemory(binarySerializer, entityTemplatesOffset);
        pinConnections.SerializeToMemory(binarySerializer, pinConnectionsOffset);
        inputPinForwardings.SerializeToMemory(binarySerializer, inputPinForwardingsOffset);
        outputPinForwardings.SerializeToMemory(binarySerializer, outputPinForwardingsOffset);
    }

    STemplateEntityBlueprint* STemplateEntityBlueprint::DeserializeFromJSON(const rapidjson::Document& document)
    {
        STemplateEntityBlueprint* templateEntityBlueprint = new STemplateEntityBlueprint();

        templateEntityBlueprint->rootEntityIndex = document["rootEntityIndex"].GetInt();
        templateEntityBlueprint->entityTemplates = *TArray<STemplateSubEntityBlueprint>::DeserializeFromJSON(document["entityTemplates"].GetArray());
        templateEntityBlueprint->pinConnections = *TArray<SEntityTemplatePinConnection>::DeserializeFromJSON(document["pinConnections"].GetArray());
        templateEntityBlueprint->inputPinForwardings = *TArray<SEntityTemplatePinConnection>::DeserializeFromJSON(document["inputPinForwardings"].GetArray());
        templateEntityBlueprint->outputPinForwardings = *TArray<SEntityTemplatePinConnection>::DeserializeFromJSON(document["outputPinForwardings"].GetArray());

        return templateEntityBlueprint;
    }
}
