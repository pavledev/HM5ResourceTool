#pragma once

#include "SPackageListDataSection.hpp"

namespace IOI
{
	struct SPackageListDataFolder
	{
		ZString name;
		TArray<SPackageListDataSection> sections;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
