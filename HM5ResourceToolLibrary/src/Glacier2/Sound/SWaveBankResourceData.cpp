#include "pch.h"
#include "Glacier2/Sound/SWaveBankResourceData.hpp"

namespace IOI
{
    std::string SWaveBankResourceData::SerializeToJSON()
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        writer.String("m_nTracks");
        writer.Int(m_nTracks);

        writer.String("m_eLoadType");
        writer.Int(m_eLoadType);

        writer.String("m_eFormat");
        writer.Int(m_eFormat);

        writer.String("m_nIsPlaceholder");
        writer.Int(m_nIsPlaceholder);

        writer.String("m_nIsNotDucking");
        writer.Int(m_nIsNotDucking);

        writer.String("m_fDuration");
        writer.Double(m_fDuration);

        writer.String("m_nChannels");
        writer.Uint(m_nChannels);

        writer.String("m_nQuality");
        writer.Int(m_nQuality);

        writer.EndObject();

        return stringBuffer.GetString();
    }

    SWaveBankResourceData* SWaveBankResourceData::DeserializeFromJSON(const rapidjson::Document& document)
    {
        SWaveBankResourceData* waveBankResourceData = new SWaveBankResourceData();

        waveBankResourceData->m_nTracks = document["m_nTracks"].GetInt();
        waveBankResourceData->m_eLoadType = static_cast<EWaveBankMemoryLoadRTType>(document["m_eLoadType"].GetInt());
        waveBankResourceData->m_eFormat = static_cast<EWaveBankRTFormat>(document["m_eFormat"].GetInt());
        waveBankResourceData->m_nIsPlaceholder = document["m_nIsPlaceholder"].GetInt();
        waveBankResourceData->m_nIsNotDucking = document["m_nIsNotDucking"].GetInt();
        waveBankResourceData->m_fDuration = document["m_fDuration"].GetFloat();
        waveBankResourceData->m_nChannels = document["m_nChannels"].GetUint();
        waveBankResourceData->m_nQuality = document["m_nQuality"].GetInt();

        return waveBankResourceData;
    }
}
