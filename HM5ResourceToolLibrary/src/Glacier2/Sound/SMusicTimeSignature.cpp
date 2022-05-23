#include "pch.h"
#include "Glacier2/Sound/SMusicTimeSignature.hpp"

namespace IOI
{
    void SMusicTimeSignature::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_nBeatsPerMinute");
        writer.Int(m_nBeatsPerMinute);

        writer.String("m_nBeatsPerBar");
        writer.Int(m_nBeatsPerBar);

        writer.EndObject();
    }
}
