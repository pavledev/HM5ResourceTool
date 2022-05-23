#pragma once

#include "../ZString.hpp"
#include "../TArray.hpp"
#include "SMusicTrackData.hpp"
#include "SMusicTimeSignature.hpp"

namespace IOI
{
	struct SMusicSegmentData
	{
		ZString m_sName;
		int m_nBarCount;
		TArray<SMusicTrackData> m_aTracks;
		SMusicTimeSignature m_signature;
		int m_nChoppedSampleRate;
		int m_nChoppedSampleCount;
		int m_nLeadInSamples;
		int m_nLeadOutSamples;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
