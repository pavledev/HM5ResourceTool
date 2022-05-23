#pragma once

#include "rapidjson/prettywriter.h"

namespace IOI
{
	struct SMusicPlaylistEndBehavior
	{
		bool m_bGotoPrevious;
		int m_nAttenuationOffset;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
