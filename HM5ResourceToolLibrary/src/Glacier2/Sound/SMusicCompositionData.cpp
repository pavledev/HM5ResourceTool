#include "pch.h"
#include "Glacier2/Sound/SMusicCompositionData.hpp"

namespace IOI
{
    std::string SMusicCompositionData::SerializeToJSON()
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

        writer.StartObject();

        writer.String("m_aGameCues");
        m_aGameCues.SerializeToJSON(writer);

        writer.String("m_aPlaylists");
        m_aPlaylists.SerializeToJSON(writer);

        writer.String("m_aStingers");
        m_aStingers.SerializeToJSON(writer);

        writer.String("m_aSegments");
        m_aSegments.SerializeToJSON(writer);

        writer.String("m_aEndBehaviors");
        m_aEndBehaviors.SerializeToJSON(writer);

        writer.String("m_transitionMap");
        m_transitionMap.SerializeToJSON(writer);

        writer.String("m_nJumpCue");
        writer.Int(m_nJumpCue);

        writer.String("m_aTrackExposingGroups");
        m_aTrackExposingGroups.SerializeToJSON(writer);

        writer.EndObject();

        return stringBuffer.GetString();
    }
}
