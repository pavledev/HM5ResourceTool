#pragma once

#include "EMusicPlayStartType.hpp"
#include "../ZCurve.hpp"

namespace IOI
{
	struct SMusicStingerData
	{
		int m_nSegmentIndex;
		EMusicPlayStartType m_ePlayStartType;
		bool m_bPreload;
		ZCurve m_duckingCurve;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
