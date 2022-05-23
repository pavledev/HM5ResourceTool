#pragma once

#include <string>

namespace HM5ResourceTool
{
	class ZTextList
	{
	public:
		static bool ConvertTextListToText(const std::string& textListPath, std::string& output);
		__declspec(dllexport) static bool ConvertTextListToText(const void* data, const unsigned int dataSize, std::string& output);
	};
}
