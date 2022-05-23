#include "pch.h"
#include "Glacier2/BehaviorTree/SBehaviorTreeInputPinCondition.hpp"

namespace IOI
{
    void SBehaviorTreeInputPinCondition::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_sName");
        m_sName.SerializeToJSON(writer);

        writer.EndObject();
    }

    void SBehaviorTreeInputPinCondition::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int nameOffset = offset + offsetof(SBehaviorTreeInputPinCondition, m_sName);

        m_sName.SerializeToMemory(binarySerializer, nameOffset);
    }

    SBehaviorTreeInputPinCondition* SBehaviorTreeInputPinCondition::DeserializeFromJSON(const rapidjson::Value& object)
    {
        SBehaviorTreeInputPinCondition* behaviorTreeInputPinCondition = new SBehaviorTreeInputPinCondition();

        behaviorTreeInputPinCondition->m_sName = *ZString::DeserializeFromJSON(object["m_sName"].GetString());

        return behaviorTreeInputPinCondition;
    }
}
