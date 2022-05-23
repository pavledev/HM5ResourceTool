#include "pch.h"
#include <fstream>
#include <iostream>
#include "Model/ZRenderPrimitive.hpp"
#include "IO/ZBinaryReader.hpp"
#include "Model/GLTF.hpp"
#include "Glacier2/Math/SVector2.hpp"

namespace HM5ResourceTool
{
	const ZRenderPrimitive::ESubType ZRenderPrimitive::ZObjectHeader::GetSubType() const
	{
		return static_cast<ESubType>(header->lSubType);
	}

	const EPropertyFlags ZRenderPrimitive::ZObjectHeader::GetPropertyFlags() const
	{
		return static_cast<EPropertyFlags>(header->lProperties);
	}

	const unsigned char ZRenderPrimitive::ZObjectHeader::GetLODMask() const
	{
		return header->lLODMask;
	}

	const IOI::SVector3* ZRenderPrimitive::ZObjectHeader::GetBoundingBoxMin() const
	{
		return &header->vBoundingBoxMin;
	}

	const IOI::SVector3* ZRenderPrimitive::ZObjectHeader::GetBoundingBoxMax() const
	{
		return &header->vBoundingBoxMax;
	}

	void ZRenderPrimitive::ZObjectHeader::Read(IO::ZBinaryReader& binaryReader)
	{
		header = static_cast<SHeader*>(binaryReader.ReadBytes(sizeof(SHeader)));
	}

	const unsigned int ZRenderPrimitive::ZObjectDetails::GetSubMeshTableOffset() const
	{
		return details->lSubMeshTable;
	}

	const IOI::SVector4* ZRenderPrimitive::ZObjectDetails::GetMeshVertexPositionScale() const
	{
		return &details->vPosScale;
	}

	const IOI::SVector4* ZRenderPrimitive::ZObjectDetails::GetMeshVertexPositionBias() const
	{
		return &details->vPosBias;
	}

	const IOI::SVector2* ZRenderPrimitive::ZObjectDetails::GetMeshVertexUVScale() const
	{
		return &details->vTexScale;
	}

	const IOI::SVector2* ZRenderPrimitive::ZObjectDetails::GetMeshVertexUVBias() const
	{
		return &details->vTexBias;
	}

	void ZRenderPrimitive::ZObjectDetails::Read(IO::ZBinaryReader& binaryReader)
	{
		details = static_cast<SDetails*>(binaryReader.ReadBytes(sizeof(SDetails)));
	}

	const unsigned int ZRenderPrimitive::ZMeshDetails::GetVertexCount() const
	{
		return details->lNumVertices;
	}

	const unsigned int ZRenderPrimitive::ZMeshDetails::GetVerticesOffset() const
	{
		return details->lVertices;
	}

	const unsigned int ZRenderPrimitive::ZMeshDetails::GetIndexCount() const
	{
		return details->lNumIndices;
	}

	const unsigned int ZRenderPrimitive::ZMeshDetails::GetIndicesOffset() const
	{
		return details->lIndices;
	}

	const unsigned int ZRenderPrimitive::ZMeshDetails::GetUVChannelCount() const
	{
		return details->lNumUVChannels;
	}

	void ZRenderPrimitive::ZMeshDetails::Read(IO::ZBinaryReader& binaryReader)
	{
		details = static_cast<SDetails*>(binaryReader.ReadBytes(sizeof(SDetails)));

		details->lNumUVChannels = 1; //Set to one, because multiple uv's are not supported yet.
	}

	ZRenderPrimitive::ZMesh::ZMesh()
	{
		header = new ZObjectHeader();
		details = new ZMeshDetails();
	}

	ZRenderPrimitive::ZMesh::~ZMesh()
	{
		delete header;
		delete details;
	}

	const ZRenderPrimitive::ZObjectHeader* ZRenderPrimitive::ZMesh::GetHeader() const
	{
		return header;
	}

	const ZRenderPrimitive::ZMeshDetails* ZRenderPrimitive::ZMesh::GetDetails() const
	{
		return details;
	}

	const std::vector<unsigned short>& ZRenderPrimitive::ZMesh::GetIndices() const
	{
		return meshBufferBuilder.GetIndices();
	}

	const std::vector<SVertex*>& ZRenderPrimitive::ZMesh::GetVertices() const
	{
		return meshBufferBuilder.GetVertices();
	}

	void ZRenderPrimitive::ZMesh::Deserialize(IO::ZBinaryReader& binaryReader, const unsigned int meshOffset)
	{
		binaryReader.GetInputStream()->Seek(meshOffset, IO::IInputStream::ESeekOrigin::Begin);
		header->Read(binaryReader);
		details->Read(binaryReader);
	}

	void ZRenderPrimitive::ZMesh::ReadBuffers(IO::ZBinaryReader& binaryReader, ZObjectHeader* objectHeader, ZObjectDetails* objectDetails)
	{
		EPropertyFlags propertyFlags = objectHeader->GetPropertyFlags();
		ESubType subType = objectHeader->GetSubType();
		bool isHighRes = (propertyFlags & EPropertyFlags::hiResPositions) == EPropertyFlags::hiResPositions;
		bool hasColor1 = (propertyFlags & EPropertyFlags::color1) == EPropertyFlags::color1;
		bool isWeighted = subType == ESubType::weighted;
		bool isLinked = subType == ESubType::linked;

		meshBufferBuilder = ZMeshBufferBuilder(details->GetIndexCount(), details->GetVertexCount(), details->GetUVChannelCount());

		if (isWeighted || isLinked)
		{
			binaryReader.GetInputStream()->Seek(details->GetIndicesOffset(), IO::IInputStream::ESeekOrigin::Begin);
			meshBufferBuilder.ReadIndices(binaryReader);
			binaryReader.GetInputStream()->Seek(details->GetVerticesOffset(), IO::IInputStream::ESeekOrigin::Begin);

			for (unsigned int i = 0; i < details->GetVertexCount(); ++i)
			{
				meshBufferBuilder.ReadVertexPosition(binaryReader, i, isHighRes, objectDetails->GetMeshVertexPositionScale(), objectDetails->GetMeshVertexPositionBias());
			}

			if (isWeighted)
			{
				for (unsigned int i = 0; i < details->GetVertexCount(); ++i)
				{
					meshBufferBuilder.ReadVertexJointAndWeight(binaryReader, i);
				}
			}

			for (unsigned int i = 0; i < details->GetVertexCount(); i++)
			{
				meshBufferBuilder.ReadVertexNormal(binaryReader, i);
				meshBufferBuilder.ReadVertexTangent(binaryReader, i);
				meshBufferBuilder.ReadVertexBitangent(binaryReader, i);

				for (unsigned char uv = 0; uv < details->GetUVChannelCount(); ++uv)
				{
					meshBufferBuilder.ReadVertexUVs(binaryReader, i, objectDetails->GetMeshVertexUVScale(), objectDetails->GetMeshVertexUVBias());
				}

				meshBufferBuilder.ReadVertexColor(binaryReader, i);
			}
		}
		else
		{
			binaryReader.GetInputStream()->Seek(details->GetIndicesOffset(), IO::IInputStream::ESeekOrigin::Begin);
			meshBufferBuilder.ReadIndices(binaryReader);
			binaryReader.GetInputStream()->Seek(details->GetVerticesOffset(), IO::IInputStream::ESeekOrigin::Begin);

			for (unsigned int i = 0; i < details->GetVertexCount(); ++i)
			{
				meshBufferBuilder.ReadVertexPosition(binaryReader, i, isHighRes, objectDetails->GetMeshVertexPositionScale(), objectDetails->GetMeshVertexPositionBias());
				meshBufferBuilder.ReadVertexNormal(binaryReader, i);
				meshBufferBuilder.ReadVertexColor(binaryReader, i);
				meshBufferBuilder.ReadVertexTangent(binaryReader, i);
				meshBufferBuilder.ReadVertexBitangent(binaryReader, i);
				meshBufferBuilder.ReadVertexUVs(binaryReader, i, objectDetails->GetMeshVertexUVScale(), objectDetails->GetMeshVertexUVBias());
			}
		}
	}

	ZRenderPrimitive::ZObject::ZObject()
	{
		header = new ZObjectHeader();
		details = new ZObjectDetails();
		mesh = new ZMesh();
	}

	ZRenderPrimitive::ZObject::~ZObject()
	{
		delete header;
		delete details;
		delete mesh;
	}

	const ZRenderPrimitive::ZObjectHeader* ZRenderPrimitive::ZObject::GetHeader() const
	{
		return header;
	}

	const ZRenderPrimitive::ZObjectDetails* ZRenderPrimitive::ZObject::GetDetails() const
	{
		return details;
	}

	const ZRenderPrimitive::ZMesh* ZRenderPrimitive::ZObject::GetMesh() const
	{
		return mesh;
	}

	void ZRenderPrimitive::ZObject::Deserialize(IO::ZBinaryReader& binaryReader, const unsigned int headerOffset)
	{
		binaryReader.GetInputStream()->Seek(headerOffset, IO::IInputStream::ESeekOrigin::Begin);
		header->Read(binaryReader);
		details->Read(binaryReader);

		binaryReader.GetInputStream()->Seek(details->GetSubMeshTableOffset(), IO::IInputStream::ESeekOrigin::Begin);

		unsigned int meshOffset = binaryReader.ReadUInt32();

		mesh->Deserialize(binaryReader, meshOffset);
		mesh->ReadBuffers(binaryReader, header, details);
	}

	ZRenderPrimitive::ZRenderPrimitive()
	{
		primaryOffset = 0;
		primaryHeader = new ZPrimaryHeader();
	}

	ZRenderPrimitive::~ZRenderPrimitive()
	{
		delete primaryHeader;
	}

	const ZRenderPrimitive::ZPrimaryHeader* ZRenderPrimitive::GetPriaryHeader() const
	{
		return primaryHeader;
	}

	const std::vector<ZRenderPrimitive::ZObject*>& ZRenderPrimitive::GetObjects() const
	{
		return objects;
	}

	void ZRenderPrimitive::Deserialize(const std::string& filePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(filePath);

		Deserialize(binaryReader);
	}

	void ZRenderPrimitive::Deserialize(const void* buffer, const unsigned int dataSize)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(buffer, dataSize);

		Deserialize(binaryReader);
	}

	void ZRenderPrimitive::Deserialize(IO::ZBinaryReader& binaryReader)
	{
		primaryOffset = binaryReader.ReadUInt32();

		binaryReader.GetInputStream()->Seek(primaryOffset, IO::IInputStream::ESeekOrigin::Begin);
		binaryReader.ReadBytes(primaryHeader, sizeof(ZPrimaryHeader));

		unsigned int objectCount = primaryHeader->GetObjectCount();
		std::vector<unsigned int> objectOffsets;

		objectOffsets.reserve(objectCount);

		binaryReader.GetInputStream()->Seek(primaryHeader->GetObjectTableOffset(), IO::IInputStream::ESeekOrigin::Begin);

		for (unsigned int i = 0; i < objectCount; ++i)
		{
			unsigned int objectOffset = binaryReader.ReadUInt32();

			objectOffsets.push_back(objectOffset);
		}

		for (unsigned int i = 0; i < objectCount; ++i)
		{
			ZObject* object = new ZObject();

			object->Deserialize(binaryReader, objectOffsets[i]);
			objects.push_back(object);
		}
	}

	void ZRenderPrimitive::ConvertToOBJ(const std::string& objFilePath)
	{
		std::ofstream ofstream = std::ofstream(objFilePath, std::ofstream::binary);

		if (!ofstream.good())
		{
			std::cout << "Error: OBJ file " + objFilePath + " could not be created.";

			return;
		}

		std::string objFileData;

		for (unsigned int i = 0; i < objects.size(); ++i)
		{
			ZObject* object = objects[i];
			std::vector<SVertex*> vertices = object->GetMesh()->GetVertices();

			for (unsigned int j = 0; j < vertices.size(); ++j)
			{
				objFileData.append("v ");
				objFileData.append(ZUtility::ConvertValueToString(vertices[j]->position->x));
				objFileData.push_back(' ');
				objFileData.append(ZUtility::ConvertValueToString(vertices[j]->position->y));
				objFileData.push_back(' ');
				objFileData.append(ZUtility::ConvertValueToString(vertices[j]->position->z));
				objFileData.push_back('\n');
			}
		}

		for (unsigned int i = 0; i < objects.size(); ++i)
		{
			ZObject* object = objects[i];
			std::vector<SVertex*> vertices = object->GetMesh()->GetVertices();

			for (unsigned int j = 0; j < vertices.size(); ++j)
			{
				objFileData.append("vn ");
				objFileData.append(ZUtility::ConvertValueToString(vertices[j]->normal->x));
				objFileData.push_back(' ');
				objFileData.append(ZUtility::ConvertValueToString(vertices[j]->normal->y));
				objFileData.push_back(' ');
				objFileData.append(ZUtility::ConvertValueToString(vertices[j]->normal->z));
				objFileData.push_back('\n');
			}
		}

		for (unsigned int i = 0; i < objects.size(); ++i)
		{
			ZObject* object = objects[i];
			std::vector<SVertex*> vertices = object->GetMesh()->GetVertices();

			for (unsigned int j = 0; j < vertices.size(); ++j)
			{
				std::vector<IOI::SVector2*> uv = vertices[j]->uv;

				for (unsigned int k = 0; k < uv.size(); ++k)
				{
					objFileData.append("vt ");
					objFileData.append(ZUtility::ConvertValueToString(uv[k]->x));
					objFileData.push_back(' ');
					objFileData.append(ZUtility::ConvertValueToString(uv[k]->y));
					objFileData.push_back('\n');
				}
			}
		}

		unsigned int vCount = 0;

		for (unsigned int i = 0; i < objects.size(); ++i)
		{
			ZObject* object = objects[i];
			std::vector<unsigned short> indices = object->GetMesh()->GetIndices();

			for (unsigned int j = 0; j < indices.size() / 3; ++j)
			{
				unsigned int one = indices[static_cast<unsigned long long>(j) * 3];

				one++;
				one += vCount;

				unsigned int two = indices[static_cast<unsigned long long>(j) * 3 + 1];

				two++;
				two += vCount;

				unsigned int three = indices[static_cast<unsigned long long>(j) * 3 + 2];

				three++;
				three += vCount;

				std::string oneString = ZUtility::ConvertValueToString(one);
				std::string twoString = ZUtility::ConvertValueToString(two);
				std::string threeString = ZUtility::ConvertValueToString(three);

				objFileData.append("f ");
				objFileData.append(oneString);
				objFileData.push_back('/');
				objFileData.append(oneString);
				objFileData.push_back('/');
				objFileData.append(oneString);
				objFileData.push_back(' ');
				objFileData.append(twoString);
				objFileData.push_back('/');
				objFileData.append(twoString);
				objFileData.push_back('/');
				objFileData.append(twoString);
				objFileData.push_back(' ');
				objFileData.append(threeString);
				objFileData.push_back('/');
				objFileData.append(threeString);
				objFileData.push_back('/');
				objFileData.append(threeString);
				objFileData.push_back('\n');
			}

			vCount += objects[i]->GetMesh()->GetVertices().size();
		}

		ofstream.write(objFileData.c_str(), objFileData.length());
		ofstream.close();
	}

	void ZRenderPrimitive::ConvertToGLTF(const std::string& gltfFilePath)
	{
		objects.shrink_to_fit();

		GLTF gltf;

		gltf.BuildGLTFModel(this, ZUtility::GetBaseNameFromPath(gltfFilePath));
		gltf.WriteGLTFToFile(gltfFilePath, false);
	}

	const unsigned int ZRenderPrimitive::ZPrimaryHeader::GetObjectCount() const
	{
		return header.lNumObjects;
	}

	const unsigned int ZRenderPrimitive::ZPrimaryHeader::GetObjectTableOffset() const
	{
		return header.lObjectTable;
	}
}
