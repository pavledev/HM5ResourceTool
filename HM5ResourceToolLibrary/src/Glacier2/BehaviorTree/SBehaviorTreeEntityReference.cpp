#include "pch.h"
#include "Glacier2/BehaviorTree/SBehaviorTreeEntityReference.hpp"

namespace IOI
{
    void SBehaviorTreeEntityReference::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_bList");
        writer.Bool(m_bList);

        writer.String("m_sName");
        m_sName.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SBehaviorTreeEntityReference::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int listOffset = offset + offsetof(SBehaviorTreeEntityReference, m_bList);
        unsigned int nameOffset = offset + offsetof(SBehaviorTreeEntityReference, m_sName);

        binarySerializer.WriteToMemory(&m_bList, sizeof(bool), listOffset);

        m_sName.SerializeToMemory(binarySerializer, nameOffset);
    }

    SBehaviorTreeEntityReference* SBehaviorTreeEntityReference::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SBehaviorTreeEntityReference* behaviorTreeEntityReference = new SBehaviorTreeEntityReference();

        behaviorTreeEntityReference->m_bList = object["m_bList"].GetBool();
        behaviorTreeEntityReference->m_sName = *ZString::DeserializeFromJSON(object["m_sName"].GetString());

        return behaviorTreeEntityReference;
    }
}
