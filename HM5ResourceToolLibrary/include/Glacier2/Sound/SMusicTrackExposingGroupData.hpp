#pragma once

#include "../ZString.hpp"
#include "../TArray.hpp"
#include "EMusicTrackExposingType.hpp"

namespace IOI
{
	struct SMusicTrackExposingGroupData
	{
		ZString m_sName;
		TArray<ZString> m_trackNames;
		TArray<EMusicTrackExposingType> m_trackTypes;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
