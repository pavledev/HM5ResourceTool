#include "pch.h"
#include "Glacier2/BehaviorTree/SBehaviorTreeInfo.hpp"

namespace IOI
{
    std::string SBehaviorTreeInfo::SerializeToJSON()
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        writer.String("m_references");
        m_references.SerializeToJSON(writer);

        writer.String("m_inputPinConditions");
        m_inputPinConditions.SerializeToJSON(writer);

        writer.EndObject();

        return stringBuffer.GetString();
    }

    void SBehaviorTreeInfo::SerializeToMemory(ZBinarySerializer& binarySerializer)
    {
        unsigned int referencesOffset = offsetof(SBehaviorTreeInfo, m_references);
        unsigned int inputPinConditionsOffset = offsetof(SBehaviorTreeInfo, m_inputPinConditions);

        m_references.SerializeToMemory(binarySerializer, referencesOffset);
        m_inputPinConditions.SerializeToMemory(binarySerializer, inputPinConditionsOffset);
    }

    SBehaviorTreeInfo* SBehaviorTreeInfo::DeserializeFromJSON(const rapidjson::Document& document)
    {
        SBehaviorTreeInfo* behaviorTreeInfo = new SBehaviorTreeInfo();

        behaviorTreeInfo->m_references = *TArray<SBehaviorTreeEntityReference>::DeserializeFromJSON(document["m_references"].GetArray());
        behaviorTreeInfo->m_inputPinConditions = *TArray<SBehaviorTreeInputPinCondition>::DeserializeFromJSON(document["m_inputPinConditions"].GetArray());

        return behaviorTreeInfo;
    }
}
