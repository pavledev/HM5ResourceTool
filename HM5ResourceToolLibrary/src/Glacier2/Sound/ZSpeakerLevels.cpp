#include "pch.h"
#include "Glacier2/Sound/ZSpeakerLevels.hpp"
#include "Glacier2/ZBinarySerializer.hpp"

namespace IOI
{
    const TArray<float>& ZSpeakerLevels::GetSpeakerMatrix() const
    {
        return m_aSpeakerMatrix;
    }

    void ZSpeakerLevels::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_aSpeakerMatrix");
        m_aSpeakerMatrix.SerializeToJSON(writer);

        writer.EndObject();
    }

    void ZSpeakerLevels::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int speakerMatrixOffset = offset + offsetof(ZSpeakerLevels, m_aSpeakerMatrix);

        m_aSpeakerMatrix.SerializeToMemory(binarySerializer, speakerMatrixOffset);
    }

    ZSpeakerLevels* ZSpeakerLevels::DeserializeFromJSON(const rapidjson::Value& object)
    {
        ZSpeakerLevels* speakerLevels = new ZSpeakerLevels();

        speakerLevels->m_aSpeakerMatrix = *TArray<float>::DeserializeFromJSON(object["m_aSpeakerMatrix"].GetArray());

        return speakerLevels;
    }

    bool ZSpeakerLevels::operator==(const ZSpeakerLevels& other)
    {
        return m_aSpeakerMatrix == other.m_aSpeakerMatrix;
    }
}
