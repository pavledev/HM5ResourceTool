#pragma once

#include "SHeaderLibraryChunk.hpp"

namespace IOI
{
	struct SHeaderLibrary
	{
		TArray<SHeaderLibraryChunk> chunks;
		TArray<ZString> externalResourceIds;
		long long ridSource;

		std::string SerializeToJSON();
		void SerializeToMemory(ZBinarySerializer& binarySerializer);
	};
}
