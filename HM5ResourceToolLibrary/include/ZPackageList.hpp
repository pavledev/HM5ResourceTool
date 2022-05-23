#pragma once

#include <string>
#include "Glacier2/ZBinaryDeserializer.hpp"
#include "Glacier2/PackageList/SPackageListData.hpp"

namespace HM5ResourceTool
{
	class ZPackageList
	{
	public:
		__declspec(dllexport) static bool ConvertPackgeListToJSON(const std::string& packageListPath, std::string& jsonOutput);
	};
}
