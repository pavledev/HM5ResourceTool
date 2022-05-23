#pragma once

#include "../TArray.hpp"
#include "SMusicTrackVariationData.hpp"

namespace IOI
{
	struct SMusicTrackEntryData
	{
		TArray<SMusicTrackVariationData> m_aVariations;
		int m_nStartBeat;
		bool m_bPlayOnce;
		int m_nSelectedVariation;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
