#include "pch.h"
#include "Glacier2/Sound/SMusicTransition.hpp"

namespace IOI
{
    void SMusicTransition::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_nSource");
        writer.Int(m_nSource);

        writer.String("m_nDestination");
        writer.Int(m_nDestination);

        writer.String("m_eStartType");
        writer.Int(m_eStartType);

        writer.String("m_eTransType");
        writer.Int(m_eTransType);

        writer.String("m_eFadeType");
        writer.Int(m_eFadeType);

        writer.String("m_fFadeTime");
        writer.Double(m_fFadeTime);

        writer.String("m_nSegmentIndex");
        writer.Int(m_nSegmentIndex);

        writer.String("m_bSkipPreEntries");
        writer.Bool(m_bSkipPreEntries);

        writer.String("m_fTransDelay");
        writer.Double(m_fTransDelay);

        writer.String("m_customFadeInCurve");
        m_customFadeInCurve.SerializeToJSON(writer);

        writer.String("m_customFadeOutCurve");
        m_customFadeOutCurve.SerializeToJSON(writer);

        writer.EndObject();
    }
}
