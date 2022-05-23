#pragma once

#include <string>

namespace HM5ResourceTool
{
	class ZResourceIndex
	{
	public:
		static bool ConvertResourceIndexToJSON(const std::string& resourceIndexPath, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertResourceIndexToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
	};
}
