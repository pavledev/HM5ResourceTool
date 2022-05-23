#pragma once

#include "../ZString.hpp"
#include "../TArray.hpp"

namespace IOI
{
	struct SHeaderLibraryChunk
	{
		ZString sLibraryID;
		unsigned int nOffset;
		char nFlags;
		unsigned int localizationCategory;
		TArray<ZString> sLanguages;
		TArray<int> states;
		TArray<TArray<unsigned char>> resourceHeaders;
		TArray<unsigned int> ridMappingIndices;
		TArray<unsigned long long> ridMappingIDs;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
		void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset);
	};
}
