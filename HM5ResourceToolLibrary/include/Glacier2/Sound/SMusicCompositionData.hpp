#pragma once

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "../TArray.hpp"
#include "SMusicGameCueData.hpp"
#include "SMusicPlaylistData.hpp"
#include "SMusicSegmentData.hpp"
#include "../TSparseShortArray.hpp"
#include "SMusicPlaylistEndBehavior.hpp"
#include "SMusicTransition.hpp"
#include "SMusicTrackExposingGroupData.hpp"

namespace IOI
{
	struct SMusicCompositionData
	{
		TArray<SMusicGameCueData> m_aGameCues;
		TArray<SMusicPlaylistData> m_aPlaylists;
		TArray<SMusicStingerData> m_aStingers;
		TArray<SMusicSegmentData> m_aSegments;
		TSparseShortArray<SMusicPlaylistEndBehavior> m_aEndBehaviors;
		TSparseShortArray<TSparseShortArray<SMusicTransition>> m_transitionMap;
		int m_nJumpCue;
		TArray<SMusicTrackExposingGroupData> m_aTrackExposingGroups;

		std::string SerializeToJSON();
	};
}
