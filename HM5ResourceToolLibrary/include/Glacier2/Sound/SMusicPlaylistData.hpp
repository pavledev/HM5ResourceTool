#pragma once

#include "../ZString.hpp"
#include "../TArray.hpp"
#include "SMusicPlaylistNodeData.hpp"
#include "SMusicStingerData.hpp"

namespace IOI
{
	struct SMusicPlaylistData
	{
		ZString m_sName;
		TArray<SMusicPlaylistNodeData> m_aNodes;
		TArray<SMusicStingerData> m_aBursts;
		bool m_bPlayBursts;
		float m_fMinBurstDelay;
		float m_fMaxBurstDelay;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
