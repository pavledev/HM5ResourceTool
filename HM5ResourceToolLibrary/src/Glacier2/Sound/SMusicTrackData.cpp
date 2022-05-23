#include "pch.h"
#include "Glacier2/Sound/SMusicTrackData.hpp"

namespace IOI
{
    void SMusicTrackData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_aEntries");
        m_aEntries.SerializeToJSON(writer);

        writer.String("m_sName");
        m_sName.SerializeToJSON(writer);

        writer.String("m_fAttenuation");
        writer.Double(m_fAttenuation);

        writer.String("m_nLowpassCutoff");
        writer.Int(m_nLowpassCutoff);

        writer.String("m_fProbability");
        writer.Double(m_fProbability);

        writer.String("m_sTrackID");
        m_sTrackID.SerializeToJSON(writer);

        writer.EndObject();
    }
}
