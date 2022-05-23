#include "pch.h"
#include "Glacier2/Sound/SMusicTrackVariationData.hpp"

namespace IOI
{
    void SMusicTrackVariationData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_nWeight");
        writer.Int(m_nWeight);

        writer.String("m_nWaveIndex");
        writer.Int(m_nWaveIndex);

        writer.EndObject();
    }
}
