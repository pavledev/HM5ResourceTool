#pragma once

#include "rapidjson/prettywriter.h"

namespace IOI
{
	struct SMusicTrackVariationData
	{
		int m_nWeight;
		int m_nWaveIndex;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
