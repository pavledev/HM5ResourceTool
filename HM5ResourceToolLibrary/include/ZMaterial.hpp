#pragma once

#include <string>
#include "ZResource.hpp"

namespace HM5ResourceTool
{
	class ZMaterial
	{
	public:
		__declspec(dllexport) static bool ConvertRenderMaterialInstanceToJSON(const std::string& matiPath, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertRenderMaterialInstanceToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertRenderMaterialEntityToJSON(const std::string& mattPath, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertRenderMaterialEntityToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertRenderMaterialEntityBlueprintToJSON(const std::string& matbPath, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertRenderMaterialEntityBlueprintToJSON(const void* data, const unsigned int dataSize, std::string& jsonOutput);
		__declspec(dllexport) static bool ConvertRenderMaterialEntityAndBlueprintToJSON(const ZResource* mattResource, std::string& jsonOutput);
		static unsigned int GetIndexOfMaterialEntityBlueprintReference(const ZResource* mattResource);
	};
}
