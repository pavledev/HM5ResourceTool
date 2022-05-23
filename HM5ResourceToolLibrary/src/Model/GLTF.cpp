#include "pch.h"
#include "Model/GLTF.hpp"
#include "Glacier2/Math/SVector2.hpp"
#include "Glacier2/Math/SVector3.hpp"
#include "Glacier2/Math/SVector4.hpp"
#include "Glacier2/SColorRGBA.hpp"

#ifndef STBI_MSC_SECURE_CRT
#define STBI_MSC_SECURE_CRT
#endif

#ifdef TINYGLTF_IMPLEMENTATION
#error "TINYGLTF_IMPLEMENTATION should only be defined once"
#else
#define TINYGLTF_IMPLEMENTATION
#endif

#ifdef STB_IMAGE_IMPLEMENTATION
#error "STB_IMAGE_IMPLEMENTATION should only be defined once"
#else
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifdef STB_IMAGE_WRITE_IMPLEMENTATION
#error "STB_IMAGE_WRITE_IMPLEMENTATION should only be defined once"
#else
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "tinygltf/tiny_gltf.h"

const tinygltf::Model* GLTF::GetModel() const
{
	return gltfModel;
}

void GLTF::SetModel(tinygltf::Model* gltfModel)
{
	this->gltfModel = gltfModel;
}

void GLTF::BuildGLTFModel(HM5ResourceTool::ZRenderPrimitive* renderPrimitive, const std::string& name)
{
	tinygltf::Model m;
	tinygltf::Scene scene;

	int atBufferViewNum = 0;
	const std::vector<HM5ResourceTool::ZRenderPrimitive::ZObject*> primObjects = renderPrimitive->GetObjects();

	for (unsigned int i = 0; i < primObjects.size(); ++i)
	{
		unsigned int indexCount = primObjects[i]->GetMesh()->GetDetails()->GetIndexCount();
		unsigned int vertexCount = primObjects[i]->GetMesh()->GetDetails()->GetVertexCount();
		unsigned int uvChannelCount = primObjects[i]->GetMesh()->GetDetails()->GetUVChannelCount();

		size_t atBufferOffset = 0;
		tinygltf::Buffer buffer;

		buffer.data = BuildGLTFBuffer(*renderPrimitive->GetPriaryHeader(), renderPrimitive->GetObjects(), i);

		m.buffers.push_back(buffer);

		tinygltf::BufferView bufferViewIndices;

		bufferViewIndices.name = "indices " + std::to_string(i);
		bufferViewIndices.buffer = i;
		bufferViewIndices.byteOffset = atBufferOffset;
		bufferViewIndices.byteLength = static_cast<size_t>(indexCount * 2);
		bufferViewIndices.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
		atBufferOffset += bufferViewIndices.byteLength;

		if ((indexCount * 2) % 4 != 0)
		{
			atBufferOffset += 2;
		}

		m.bufferViews.push_back(bufferViewIndices);

		tinygltf::BufferView bufferViewVertPos;

		bufferViewVertPos.buffer = i;
		bufferViewVertPos.name = "vertexPos " + std::to_string(i);
		bufferViewVertPos.byteOffset = atBufferOffset;
		bufferViewVertPos.byteLength = static_cast<size_t>(vertexCount * 12);
		bufferViewVertPos.target = TINYGLTF_TARGET_ARRAY_BUFFER;
		atBufferOffset += bufferViewVertPos.byteLength;

		m.bufferViews.push_back(bufferViewVertPos);

		tinygltf::BufferView bufferViewVertNorm;

		bufferViewVertNorm.buffer = i;
		bufferViewVertNorm.name = "vertexNorm_" + std::to_string(i);
		bufferViewVertNorm.byteOffset = atBufferOffset;
		bufferViewVertNorm.byteLength = static_cast<size_t>(vertexCount * 12);
		bufferViewVertNorm.target = TINYGLTF_TARGET_ARRAY_BUFFER;
		atBufferOffset += bufferViewVertNorm.byteLength;

		m.bufferViews.push_back(bufferViewVertNorm);

		tinygltf::BufferView bufferViewVertTan;

		bufferViewVertTan.buffer = i;
		bufferViewVertTan.name = "vertexTangent_" + std::to_string(i);
		bufferViewVertTan.byteOffset = atBufferOffset;
		bufferViewVertTan.byteLength = static_cast<size_t>(vertexCount * 16);
		bufferViewVertTan.target = TINYGLTF_TARGET_ARRAY_BUFFER;
		atBufferOffset += bufferViewVertTan.byteLength;

		m.bufferViews.push_back(bufferViewVertTan);

		std::vector<tinygltf::BufferView> bufferViewUVs = std::vector<tinygltf::BufferView>(uvChannelCount);

		for (size_t uv = 0; uv < uvChannelCount; ++uv)
		{
			bufferViewUVs[uv].buffer = i;
			bufferViewUVs[uv].name = "vertexUv_" + std::to_string(i);
			bufferViewUVs[uv].byteOffset = atBufferOffset;
			bufferViewUVs[uv].byteLength = static_cast<size_t>(vertexCount * 8);
			bufferViewUVs[uv].target = TINYGLTF_TARGET_ARRAY_BUFFER;
			atBufferOffset += bufferViewUVs[uv].byteLength;

			m.bufferViews.push_back(bufferViewUVs[uv]);
		}

		tinygltf::BufferView bufferViewVertColor;

		bufferViewVertColor.buffer = i;
		bufferViewVertColor.name = "vertexColor_" + std::to_string(i);
		bufferViewVertColor.byteOffset = atBufferOffset;
		bufferViewVertColor.byteLength = static_cast<size_t>(vertexCount * 4);
		bufferViewVertColor.target = TINYGLTF_TARGET_ARRAY_BUFFER;
		atBufferOffset += bufferViewVertColor.byteLength;

		m.bufferViews.push_back(bufferViewVertColor);

		if (renderPrimitive->GetObjects()[i]->GetHeader()->GetSubType() == HM5ResourceTool::ZRenderPrimitive::ESubType::weighted)
		{
			tinygltf::BufferView bufferViewJoint0;
			bufferViewJoint0.buffer = i;
			bufferViewJoint0.name = "Joints_0_" + std::to_string(i);
			bufferViewJoint0.byteOffset = atBufferOffset;
			bufferViewJoint0.byteLength = static_cast<size_t>(vertexCount * 4);
			bufferViewJoint0.target = TINYGLTF_TARGET_ARRAY_BUFFER;
			atBufferOffset += bufferViewJoint0.byteLength;
			m.bufferViews.push_back(bufferViewJoint0);

			tinygltf::BufferView bufferViewJoint1;
			bufferViewJoint1.buffer = i;
			bufferViewJoint1.name = "joints_1_" + std::to_string(i);
			bufferViewJoint1.byteOffset = atBufferOffset;
			bufferViewJoint1.byteLength = static_cast<size_t>(vertexCount * 4);
			bufferViewJoint1.target = TINYGLTF_TARGET_ARRAY_BUFFER;
			atBufferOffset += bufferViewJoint1.byteLength;
			m.bufferViews.push_back(bufferViewJoint1);

			tinygltf::BufferView bufferViewWeight;
			bufferViewWeight.buffer = i;
			bufferViewWeight.name = "vertexWeight_" + std::to_string(i);
			bufferViewWeight.byteOffset = atBufferOffset;
			bufferViewWeight.byteLength = static_cast<size_t>(vertexCount * 16);
			bufferViewWeight.target = TINYGLTF_TARGET_ARRAY_BUFFER;
			atBufferOffset += bufferViewWeight.byteLength;
			m.bufferViews.push_back(bufferViewWeight);

			tinygltf::BufferView bufferViewWeight2;
			bufferViewWeight2.buffer = i;
			bufferViewWeight2.name = "vertexWeight2_" + std::to_string(i);
			bufferViewWeight2.byteOffset = atBufferOffset;
			bufferViewWeight2.byteLength = static_cast<size_t>(vertexCount * 16);
			bufferViewWeight2.target = TINYGLTF_TARGET_ARRAY_BUFFER;
			atBufferOffset += bufferViewWeight2.byteLength;

			m.bufferViews.push_back(bufferViewWeight2);
		}

		tinygltf::Accessor indicesAccessor;
		indicesAccessor.bufferView = atBufferViewNum++;
		indicesAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
		indicesAccessor.count = indexCount;
		indicesAccessor.type = TINYGLTF_TYPE_SCALAR;

		m.accessors.push_back(indicesAccessor);

		const IOI::SVector3* boundingBoxMin = renderPrimitive->GetObjects()[i]->GetHeader()->GetBoundingBoxMin();
		const IOI::SVector3* boundingBoxMax = renderPrimitive->GetObjects()[i]->GetHeader()->GetBoundingBoxMax();

		tinygltf::Accessor vertPosAccessor;
		vertPosAccessor.bufferView = atBufferViewNum++;
		vertPosAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
		vertPosAccessor.count = vertexCount;
		vertPosAccessor.type = TINYGLTF_TYPE_VEC3;
		vertPosAccessor.minValues = { boundingBoxMin->x, boundingBoxMin->y, boundingBoxMin->z };
		vertPosAccessor.maxValues = { boundingBoxMax->x, boundingBoxMax->y, boundingBoxMax->z };

		m.accessors.push_back(vertPosAccessor);

		tinygltf::Accessor vertNormAccessor;
		vertNormAccessor.bufferView = atBufferViewNum++;
		vertNormAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
		vertNormAccessor.count = vertexCount;
		vertNormAccessor.type = TINYGLTF_TYPE_VEC3;

		m.accessors.push_back(vertNormAccessor);

		tinygltf::Accessor vertTangentAccessor;
		vertTangentAccessor.bufferView = atBufferViewNum++;
		vertTangentAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
		vertTangentAccessor.count = vertexCount;
		vertTangentAccessor.type = TINYGLTF_TYPE_VEC4;

		m.accessors.push_back(vertTangentAccessor);

		std::vector<tinygltf::Accessor> vertUvAccessors = std::vector<tinygltf::Accessor>(uvChannelCount);

		for (size_t uv = 0; uv < uvChannelCount; ++uv)
		{
			vertUvAccessors[uv].bufferView = atBufferViewNum++;
			vertUvAccessors[uv].componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
			vertUvAccessors[uv].count = vertexCount;
			vertUvAccessors[uv].type = TINYGLTF_TYPE_VEC2;

			m.accessors.push_back(vertUvAccessors[uv]);
		}

		tinygltf::Accessor vertColorAccessor;

		vertColorAccessor.bufferView = atBufferViewNum++;
		vertColorAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
		vertColorAccessor.count = vertexCount;
		vertColorAccessor.normalized = true;
		vertColorAccessor.type = TINYGLTF_TYPE_VEC4;

		m.accessors.push_back(vertColorAccessor);

		tinygltf::Accessor jointAccessor;
		tinygltf::Accessor jointAccessor2;
		tinygltf::Accessor vertWeightAccessor;
		tinygltf::Accessor vertWeightAccessor2;

		if (renderPrimitive->GetObjects()[i]->GetHeader()->GetSubType() == HM5ResourceTool::ZRenderPrimitive::ESubType::weighted)
		{
			jointAccessor.bufferView = atBufferViewNum++;
			jointAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
			jointAccessor.count = vertexCount;
			jointAccessor.type = TINYGLTF_TYPE_VEC4;

			m.accessors.push_back(jointAccessor);

			jointAccessor2.bufferView = atBufferViewNum++;
			jointAccessor2.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
			jointAccessor2.count = vertexCount;
			jointAccessor2.type = TINYGLTF_TYPE_VEC4;

			m.accessors.push_back(jointAccessor2);

			vertWeightAccessor.bufferView = atBufferViewNum++;
			vertWeightAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
			vertWeightAccessor.count = vertexCount;
			vertWeightAccessor.type = TINYGLTF_TYPE_VEC4;

			m.accessors.push_back(vertWeightAccessor);

			vertWeightAccessor2.bufferView = atBufferViewNum++;
			vertWeightAccessor2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
			vertWeightAccessor2.count = vertexCount;
			vertWeightAccessor2.type = TINYGLTF_TYPE_VEC4;

			m.accessors.push_back(vertWeightAccessor2);
		}

		// Build the mesh primitive and add it to the mesh
		tinygltf::Primitive primitive;

		primitive.indices = indicesAccessor.bufferView;                 // The index of the accessor for the vertex indices
		primitive.attributes["POSITION"] = vertPosAccessor.bufferView;  // The index of the accessor for positions
		primitive.attributes["NORMAL"] = vertNormAccessor.bufferView;
		primitive.attributes["TANGENT"] = vertTangentAccessor.bufferView;

		for (size_t uv = 0; uv < uvChannelCount; uv++)
		{
			std::string label = ("TEXCOORD_" + std::to_string(unsigned(uv)));
			primitive.attributes[label] = vertUvAccessors[uv].bufferView;
		}

		primitive.attributes["COLOR_0"] = vertColorAccessor.bufferView;

		if (renderPrimitive->GetObjects()[i]->GetHeader()->GetSubType() == HM5ResourceTool::ZRenderPrimitive::ESubType::weighted)
		{
			primitive.attributes["JOINTS_0"] = jointAccessor.bufferView;
			primitive.attributes["JOINTS_1"] = jointAccessor2.bufferView;
			primitive.attributes["WEIGHTS_0"] = vertWeightAccessor.bufferView;
			primitive.attributes["WEIGHTS_1"] = vertWeightAccessor2.bufferView;
		}

		primitive.mode = TINYGLTF_MODE_TRIANGLES;

		tinygltf::Mesh mesh;

		mesh.primitives.push_back(primitive);
		mesh.name = name + "_" + std::to_string(renderPrimitive->GetObjects()[i]->GetHeader()->GetLODMask());
		m.meshes.push_back(mesh);

		tinygltf::Node meshNode;

		meshNode.mesh = i;
		m.nodes.push_back(meshNode);

		scene.nodes.push_back(i);
	}

	tinygltf::Node entryNode;

	entryNode.name = name;

	for (unsigned int i = 0; i < renderPrimitive->GetObjects().size(); ++i)
	{
		entryNode.children.push_back(i);
	}

	entryNode.rotation = { 0.70710677, 0.0, 0.0, -0.70710677 };

	m.nodes.push_back(entryNode);

	// Define the asset. The version is required
	tinygltf::Asset asset;

	asset.version = "2.0";
	asset.generator = "RenderPrimitiveParser";
	m.asset = asset;

	//wiring up the final elements
	m.defaultScene = 0;
	m.scenes.push_back(scene);

	tinygltf::Model* model = new tinygltf::Model(m);

	SetModel(model);
}

void GLTF::WriteGLTFToFile(const std::string& outputPath, const bool writeBinary)
{
	tinygltf::TinyGLTF gltf;

	gltf.WriteGltfSceneToFile(gltfModel, outputPath,
		true, // embedImages
		true, // embedBuffers
		true, // pretty print
		writeBinary); // write binary
}

std::vector<unsigned char> GLTF::BuildGLTFBuffer(const HM5ResourceTool::ZRenderPrimitive::ZPrimaryHeader& primaryHeader, const std::vector<HM5ResourceTool::ZRenderPrimitive::ZObject*>& objects, const int index)
{
	std::vector<unsigned char> bufferData;

	const unsigned int indexCount = objects[index]->GetMesh()->GetDetails()->GetIndexCount();
	const unsigned int vertexCount = objects[index]->GetMesh()->GetDetails()->GetVertexCount();
	const unsigned int uvChannelCount = objects[index]->GetMesh()->GetDetails()->GetUVChannelCount();
	const std::vector<unsigned short> indices = objects[index]->GetMesh()->GetIndices();
	const std::vector<SVertex*> vertices = objects[index]->GetMesh()->GetVertices();

	int reserveLength =
		indexCount * 2 + //indices
		vertexCount * 12 + //vertex pos
		vertexCount * 12 + //vertex normal
		vertexCount * 8 + //vertex UV
		vertexCount * 4; //vertex color

	if (objects[index]->GetHeader()->GetSubType() == HM5ResourceTool::ZRenderPrimitive::ESubType::weighted)
	{
		reserveLength += vertexCount * 40; //bone indices and weights
	}

	bufferData.reserve(reserveLength);

	for (unsigned int i = 0; i < indexCount; ++i)
	{
		ZUtility::AddValueToBuffer(bufferData, indices[i]);
	}

	if ((indexCount * 2) % 4 != 0)
	{
		ZUtility::AddValueToBuffer(bufferData, static_cast<unsigned short>(0));
	}

	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->position->x);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->position->y);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->position->z);
	}

	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->normal->x);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->normal->y);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->normal->z);
	}

	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->tangent->x);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->tangent->y);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->tangent->z);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->tangent->w);
	}

	for (unsigned int uv = 0; uv < uvChannelCount; ++uv)
	{
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->uv[uv]->x);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->uv[uv]->y);
		}
	}

	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->color->r);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->color->g);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->color->b);
		ZUtility::AddValueToBuffer(bufferData, vertices[i]->color->a);
	}

	if (objects[index]->GetHeader()->GetSubType() == HM5ResourceTool::ZRenderPrimitive::ESubType::weighted)
	{
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->joint.a);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->joint.b);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->joint.c);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->joint.d);
		}

		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->joint.e);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->joint.f);
			ZUtility::AddValueToBuffer(bufferData, static_cast<unsigned char>(0));
			ZUtility::AddValueToBuffer(bufferData, static_cast<unsigned char>(0));
		}

		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->weight.a);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->weight.b);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->weight.c);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->weight.d);
		}

		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->weight.e);
			ZUtility::AddValueToBuffer(bufferData, vertices[i]->weight.f);
			ZUtility::AddValueToBuffer(bufferData, 0.0f);
			ZUtility::AddValueToBuffer(bufferData, 0.0f);
		}
	}

	return bufferData;
}
