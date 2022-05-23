#pragma once

#include "../ZString.hpp"
#include "../TArray.hpp"
#include "EMusicPlaylistPlayType.hpp"

namespace IOI
{
	struct SMusicPlaylistNodeData
	{
		int m_nSegmentIndex;
		ZString m_sName;
		int m_nParentIndex;
		TArray<int> m_aChildIndices;
		EMusicPlaylistPlayType m_ePlayType;
		int m_nWeight;
		int m_nLoopCount;
		int m_NoRepeatCount;
		int m_nSelectProbability;
		int m_nMaxLoopCount;
		int m_nRealLoopCount;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
