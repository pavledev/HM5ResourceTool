#pragma once

#include <string>

namespace HM5ResourceTool
{
	class ZChartType
	{
	public:
		__declspec(dllexport) static bool ConvertChartTypeToText(const void* data, const unsigned int dataSize, std::string& output);
	};
}
