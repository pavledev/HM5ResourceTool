#pragma once

#include <string>
#include <sstream>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace IOI
{
	struct SResourceHeaderHeader
	{
		unsigned int m_type;
		unsigned int m_nReferencesChunkSize;
		unsigned int m_nStatesChunkSize;
		unsigned int m_nDataSize;
		unsigned int m_nSystemMemoryRequirement;
		unsigned int m_nVideoMemoryRequirement;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	};
}
