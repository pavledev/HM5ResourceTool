#pragma once

#include "EActorPerceptionSensitivity.hpp"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

namespace IOI
{
    class ZBinarySerializer;

    class ZSharedSensorDef
    {
    public:
        struct SVisibilitySetting
        {
            EActorPerceptionSensitivity m_eSensitivity;
            float m_fCloseRange;
            float m_fPeripheralRange;
            float m_fPeripheralAngle;
            float m_fPeripheralHigh;
            float m_fPeripheralLow;
            float m_fFocusConeWidthAngle;
            float m_fFocusConeHeightAngle;
            float m_fFocusConeRange;

            void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
            void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
            static SVisibilitySetting* DeserializeFromJSON(const rapidjson::Value& object);
            bool operator==(const SVisibilitySetting& other);
        };
    };
}
