#include "pch.h"
#include "Glacier2/Sound/SMusicPlaylistData.hpp"

namespace IOI
{
    void SMusicPlaylistData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_sName");
        m_sName.SerializeToJSON(writer);

        writer.String("m_aNodes");
        m_aNodes.SerializeToJSON(writer);

        writer.String("m_aBursts");
        m_aBursts.SerializeToJSON(writer);

        writer.String("m_bPlayBursts");
        writer.Bool(m_bPlayBursts);

        writer.String("m_fMinBurstDelay");
        writer.Double(m_fMinBurstDelay);

        writer.String("m_fMaxBurstDelay");
        writer.Double(m_fMaxBurstDelay);

        writer.EndObject();
    }
}
