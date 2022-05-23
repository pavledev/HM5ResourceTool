#include "pch.h"
#include "ZPackageList.hpp"

namespace HM5ResourceToolCLI
{
	bool ZPackageList::ConvertPackgeListToJSON(String^ packageListPath, String^% jsonOutput)
	{
		std::string packageListPath2 = marshal_as<std::string>(packageListPath);
		std::string jsonOutput2;

		bool result = HM5ResourceTool::ZPackageList::ConvertPackgeListToJSON(packageListPath2, jsonOutput2);

		jsonOutput = gcnew String(jsonOutput2.c_str());

		return result;
	}
}
