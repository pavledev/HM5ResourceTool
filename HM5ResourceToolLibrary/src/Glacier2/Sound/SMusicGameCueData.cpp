#include "pch.h"
#include "Glacier2/Sound/SMusicGameCueData.hpp"

namespace IOI
{
    void SMusicGameCueData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_sName");
        m_sName.SerializeToJSON(writer);

        writer.String("m_bIsStinger");
        writer.Bool(m_bIsStinger);

        writer.String("m_nTargetIndex");
        writer.Int(m_nTargetIndex);

        writer.EndObject();
    }
}
