#include "pch.h"
#include "Glacier2/Sound/SMusicTrackEntryData.hpp"

namespace IOI
{
    void SMusicTrackEntryData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_aVariations");
        m_aVariations.SerializeToJSON(writer);

        writer.String("m_nStartBeat");
        writer.Int(m_nStartBeat);

        writer.String("m_bPlayOnce");
        writer.Bool(m_bPlayOnce);

        writer.String("m_nSelectedVariation");
        writer.Int(m_nSelectedVariation);

        writer.EndObject();
    }
}
