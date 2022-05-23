#pragma once

#include "rapidjson/prettywriter.h"
#include "EMusicPlayStartType.hpp"
#include "EMusicTransitionType.hpp"
#include "EMusicTransitionFadeType.hpp"
#include "../ZCurve.hpp"

namespace IOI
{
	struct SMusicTransition
	{
		int m_nSource;
		int m_nDestination;
		EMusicPlayStartType m_eStartType;
		EMusicTransitionType m_eTransType;
		EMusicTransitionFadeType m_eFadeType;
		float m_fFadeTime;
		int m_nSegmentIndex;
		bool m_bSkipPreEntries;
		float m_fTransDelay;
		ZCurve m_customFadeInCurve;
		ZCurve m_customFadeOutCurve;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
