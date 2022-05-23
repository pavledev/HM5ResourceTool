#include "pch.h"
#include "Glacier2/Sound/SMusicPlaylistEndBehavior.hpp"

namespace IOI
{
    void SMusicPlaylistEndBehavior::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_bGotoPrevious");
        writer.Bool(m_bGotoPrevious);

        writer.String("m_nAttenuationOffset");
        writer.Int(m_nAttenuationOffset);

        writer.EndObject();
    }
}
