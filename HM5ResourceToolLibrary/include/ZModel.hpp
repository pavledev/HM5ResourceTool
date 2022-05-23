#pragma once

#include <string>

namespace HM5ResourceTool
{
	class ZModel
	{
	public:
		__declspec(dllexport) static bool ConvertRenderPrimitiveToOBJ(const std::string& primFilePath, const std::string& objFilePath);
		__declspec(dllexport) static bool ConvertRenderPrimitiveToOBJ(const void* data, const unsigned int dataSize, const std::string& objFilePath);
		__declspec(dllexport) static bool ConvertRenderPrimitiveToGLTF(const std::string& primFilePath, const std::string& gltfFilePath);
		__declspec(dllexport) static bool ConvertRenderPrimitiveToGLTF(const void* data, const unsigned int dataSize, const std::string& gltfFilePath);
	};
}
