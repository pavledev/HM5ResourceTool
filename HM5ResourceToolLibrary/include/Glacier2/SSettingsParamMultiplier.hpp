#pragma once

#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "ESettingsParam.hpp"

namespace IOI
{
    class ZBinarySerializer;

    struct SSettingsParamMultiplier
    {
        ESettingsParam m_eSettingsParam;
        float m_fMultiplier;

        void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
        static SSettingsParamMultiplier* DeserializeFromJSON(const rapidjson::Value& object);
    };
}
