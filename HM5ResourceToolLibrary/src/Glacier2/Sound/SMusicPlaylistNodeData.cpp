#include "pch.h"
#include "Glacier2/Sound/SMusicPlaylistNodeData.hpp"

namespace IOI
{
    void SMusicPlaylistNodeData::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_nSegmentIndex");
        writer.Int(m_nSegmentIndex);

        writer.String("m_sName");
        m_sName.SerializeToJSON(writer);

        writer.String("m_nParentIndex");
        writer.Int(m_nParentIndex);

        writer.String("m_aChildIndices");
        m_aChildIndices.SerializeToJSON(writer);

        writer.String("m_ePlayType");
        writer.Int(m_ePlayType);

        writer.String("m_nWeight");
        writer.Int(m_nWeight);

        writer.String("m_nLoopCount");
        writer.Int(m_nLoopCount);

        writer.String("m_NoRepeatCount");
        writer.Int(m_NoRepeatCount);

        writer.String("m_nSelectProbability");
        writer.Int(m_nSelectProbability);

        writer.String("m_nMaxLoopCount");
        writer.Int(m_nMaxLoopCount);

        writer.String("m_nRealLoopCount");
        writer.Int(m_nRealLoopCount);

        writer.EndObject();
    }
}
