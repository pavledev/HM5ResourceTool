#pragma once

#include "rapidjson/prettywriter.h"

namespace IOI
{
	struct SMusicTimeSignature
	{
		int m_nBeatsPerMinute;
		int m_nBeatsPerBar;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
