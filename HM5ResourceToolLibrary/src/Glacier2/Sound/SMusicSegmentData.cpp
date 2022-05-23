#include "pch.h"
#include "Glacier2/Sound/SMusicSegmentData.hpp"

namespace IOI
{
    void SMusicSegmentData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_sName");
        m_sName.SerializeToJSON(writer);

        writer.String("m_nBarCount");
        writer.Int(m_nBarCount);

        writer.String("m_aTracks");
        m_aTracks.SerializeToJSON(writer);

        writer.String("m_signature");
        m_signature.SerializeToJSON(writer);

        writer.String("m_nChoppedSampleRate");
        writer.Int(m_nChoppedSampleRate);

        writer.String("m_nChoppedSampleCount");
        writer.Int(m_nChoppedSampleCount);

        writer.String("m_nLeadInSamples");
        writer.Int(m_nLeadInSamples);

        writer.String("m_nLeadOutSamples");
        writer.Int(m_nLeadOutSamples);

        writer.EndObject();
    }
}
