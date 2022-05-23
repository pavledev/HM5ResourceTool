#pragma once

#include "Glacier2/TArray.hpp"

namespace IOI
{
    class ZSpeakerLevels
    {
    public:
        const TArray<float>& GetSpeakerMatrix() const;
        void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
        static ZSpeakerLevels* DeserializeFromJSON(const rapidjson::Value& object);
        bool operator==(const ZSpeakerLevels& other);

    private:
        TArray<float> m_aSpeakerMatrix;
    };
}
