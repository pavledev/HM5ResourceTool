#include "pch.h"
#include "Model/ZMeshBufferBuilder.hpp"
#include "IO/ZBinaryReader.hpp"
#include "Glacier2/Math/SVector2.hpp"
#include "Glacier2/SColorRGBA.hpp"

#undef max

namespace HM5ResourceTool
{
	ZMeshBufferBuilder::ZMeshBufferBuilder(const unsigned int indexCount, const unsigned int vertexCount, const unsigned int uvChannelCount)
	{
		this->indexCount = indexCount;
		this->vertexCount = vertexCount;
		this->indices = std::vector<unsigned short>(indexCount);
		this->vertices = std::vector<SVertex*>(vertexCount);

		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			vertices[i] = new SVertex();
		}
	}

	const std::vector<unsigned short>& ZMeshBufferBuilder::GetIndices() const
	{
		return indices;
	}

	const std::vector<SVertex*>& ZMeshBufferBuilder::GetVertices() const
	{
		return vertices;
	}

	void ZMeshBufferBuilder::ReadIndices(HM5ResourceTool::IO::ZBinaryReader& binaryReader)
	{
		for (unsigned int i = 0; i < indexCount; ++i)
		{
			indices[i] = binaryReader.ReadUInt16();
		}
	}

	void ZMeshBufferBuilder::ReadVertexPosition(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex, const bool isHighRes, const IOI::SVector4* scale, const IOI::SVector4* bias)
	{
		if (!isHighRes)
		{
			short x = binaryReader.ReadInt16();
			short y = binaryReader.ReadInt16();
			short z = binaryReader.ReadInt16();
			short w = binaryReader.ReadInt16();

			vertices[vertexIndex]->position = new IOI::SVector4(
				static_cast<float>(x * scale->x) / std::numeric_limits<short>::max() + bias->x,
				static_cast<float>(y * scale->y) / std::numeric_limits<short>::max() + bias->y,
				static_cast<float>(z * scale->z) / std::numeric_limits<short>::max() + bias->z,
				static_cast<float>(w * scale->w) / std::numeric_limits<short>::max() + bias->w);
		}
		else
		{
			float x = binaryReader.ReadFloat();
			float y = binaryReader.ReadFloat();
			float z = binaryReader.ReadFloat();

			vertices[vertexIndex]->position = new IOI::SVector4(x, y, z, 1.0f);
		}
	}

	void ZMeshBufferBuilder::ReadVertexJointAndWeight(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex)
	{
		unsigned char wa = binaryReader.ReadByte();
		unsigned char wb = binaryReader.ReadByte();
		unsigned char wc = binaryReader.ReadByte();
		unsigned char wd = binaryReader.ReadByte();

		unsigned char ba = binaryReader.ReadByte();
		unsigned char bb = binaryReader.ReadByte();
		unsigned char bc = binaryReader.ReadByte();
		unsigned char bd = binaryReader.ReadByte();

		unsigned char we = binaryReader.ReadByte();
		unsigned char wf = binaryReader.ReadByte();
		unsigned char be = binaryReader.ReadByte();
		unsigned char bf = binaryReader.ReadByte();

		vertices[vertexIndex]->weight = SVector6<float>(
			static_cast<float>(wa) / 255,
			static_cast<float>(wb) / 255,
			static_cast<float>(wc) / 255,
			static_cast<float>(wd) / 255,
			static_cast<float>(we) / 255,
			static_cast<float>(wf) / 255);

		vertices[vertexIndex]->joint = SVector6<unsigned char>(ba, bb, bc, bd, be, bf);
	}

	void ZMeshBufferBuilder::ReadVertexNormal(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex)
	{
		vertices[vertexIndex]->normal = new IOI::SVector4(binaryReader.ReadVector4());
	}

	void ZMeshBufferBuilder::ReadVertexTangent(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex)
	{
		vertices[vertexIndex]->tangent = new IOI::SVector4(binaryReader.ReadVector4());
	}

	void ZMeshBufferBuilder::ReadVertexBitangent(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex)
	{
		vertices[vertexIndex]->biTangent = new IOI::SVector4(binaryReader.ReadVector4());
	}

	void ZMeshBufferBuilder::ReadVertexUVs(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex, const IOI::SVector2* scale, const IOI::SVector2* bias)
	{
		float x = scale->x * static_cast<float>(binaryReader.ReadInt16()) / std::numeric_limits<short>::max() + bias->x;
		float y = scale->y * static_cast<float>(binaryReader.ReadInt16()) / std::numeric_limits<short>::max() + bias->y;

		vertices[vertexIndex]->uv.push_back(new IOI::SVector2(x, y));
	}

	void ZMeshBufferBuilder::ReadVertexColor(HM5ResourceTool::IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex)
	{
		unsigned char r = binaryReader.ReadByte();
		unsigned char g = binaryReader.ReadByte();
		unsigned char b = binaryReader.ReadByte();
		unsigned char a = binaryReader.ReadByte();

		vertices[vertexIndex]->color = new ::SColorRGBA(r, g, b, a);
	}
}
