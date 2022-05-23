#pragma once

#include <vector>
#include <string>
#include "Model/ZRenderPrimitive.hpp"

namespace tinygltf
{
	class Model;
}

class GLTF
{
public:
	const tinygltf::Model* GetModel() const;
	void SetModel(tinygltf::Model* gltfModel);
	void BuildGLTFModel(HM5ResourceTool::ZRenderPrimitive* renderPrimitive, const std::string& name);
	void WriteGLTFToFile(const std::string& outputPath, const bool writeBinary = true);

private:
	tinygltf::Model* gltfModel;

	std::vector<unsigned char> BuildGLTFBuffer(const HM5ResourceTool::ZRenderPrimitive::ZPrimaryHeader& primaryHeader, const std::vector<HM5ResourceTool::ZRenderPrimitive::ZObject*>& objects, const int index);
};
