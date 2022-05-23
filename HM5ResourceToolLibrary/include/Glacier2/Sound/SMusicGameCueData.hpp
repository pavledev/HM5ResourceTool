#pragma once

#include "../ZString.hpp"

namespace IOI
{
	struct SMusicGameCueData
	{
		ZString m_sName;
		bool m_bIsStinger;
		int m_nTargetIndex;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
