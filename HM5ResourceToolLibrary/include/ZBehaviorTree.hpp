#pragma once

#include <string>
#include "Glacier2/ZBinaryDeserializer.hpp"
#include "Glacier2/BehaviorTree/SBehaviorTreeInfo.hpp"

namespace HM5ResourceTool
{
	class ZBehaviorTree
	{
	public:
		__declspec(dllexport) static bool ConvertBehaviorTreeEntityBlueprintToJSON(const std::string& behaviorTreeEntityBlueprintPath, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertBehaviorTreeEntityBlueprintToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertJSONToBehaviorTreeEntityBlueprint(const std::string& jsonPath, void** data, unsigned int& dataSize);
	};
}
