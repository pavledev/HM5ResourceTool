#pragma once

#include "../TArray.hpp"

namespace IOI
{
	struct SPackageListDataSection
	{
		TArray<ZString> sceneResourceIDs;
		TArray<ZString> resources;

		void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};
}
