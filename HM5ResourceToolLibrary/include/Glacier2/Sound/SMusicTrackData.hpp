#pragma once

#include "../TArray.hpp"
#include "SMusicTrackEntryData.hpp"

namespace IOI
{
	struct SMusicTrackData
	{
		TArray<SMusicTrackEntryData> m_aEntries;
		ZString m_sName;
		float m_fAttenuation;
		int m_nLowpassCutoff;
		float m_fProbability;
		ZString m_sTrackID;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
