#include "pch.h"
#include "Glacier2/Sound/SMusicStingerData.hpp"

namespace IOI
{
    void SMusicStingerData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_nSegmentIndex");
        writer.Int(m_nSegmentIndex);

        writer.String("m_ePlayStartType");
        writer.Int(m_ePlayStartType);

        writer.String("m_bPreload");
        writer.Bool(m_bPreload);

        writer.String("m_duckingCurve");
        m_duckingCurve.SerializeToJSON(writer);

        writer.EndObject();
    }
}
