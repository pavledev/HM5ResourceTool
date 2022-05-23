#pragma once

#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

namespace IOI
{
    class ZBinarySerializer;

    struct SBodyPartDamageMultipliers
    {
        float m_fHeadDamageMultiplier;
        float m_fFaceDamageMultiplier;
        float m_fArmDamageMultiplier;
        float m_fHandDamageMultiplier;
        float m_fLegDamageMultiplier;
        float m_fTorsoDamageMultiplier;

        void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
        static SBodyPartDamageMultipliers* DeserializeFromJSON(const rapidjson::Value& object);
        bool operator==(const SBodyPartDamageMultipliers& other);
    };
}
