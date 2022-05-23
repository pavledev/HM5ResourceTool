#pragma once

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "EWaveBankMemoryLoadRTType.hpp"
#include "EWaveBankRTFormat.hpp"

namespace IOI
{
	struct SWaveBankResourceData
	{
		int m_nTracks;
		EWaveBankMemoryLoadRTType m_eLoadType;
		EWaveBankRTFormat m_eFormat;
		int m_nIsPlaceholder;
		int m_nIsNotDucking;
		float m_fDuration;
		unsigned int m_nChannels;
		int m_nQuality;

		std::string SerializeToJSON();
		static SWaveBankResourceData* DeserializeFromJSON(const rapidjson::Document& document);
	};
}
