#include "pch.h"
#include "Glacier2/Sound/SMusicTrackExposingGroupData.hpp"

namespace IOI
{
    void SMusicTrackExposingGroupData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_sName");
        m_sName.SerializeToJSON(writer);

        writer.String("m_trackNames");
        m_trackNames.SerializeToJSON(writer);

        writer.String("m_trackTypes");
        m_trackTypes.SerializeToJSON(writer);

        writer.EndObject();
    }
}
