#pragma once

#include <vector>
#include "ZUtility.hpp"
#include "IO/ZBinaryReader.hpp"

namespace HM5ResourceTool
{
	class ZMeshBufferBuilder
	{
	public:
		ZMeshBufferBuilder() = default;
		ZMeshBufferBuilder(const unsigned int indexCount, const unsigned int vertexCount, const unsigned int uvChannelCount);
		const std::vector<unsigned short>& GetIndices() const;
		const std::vector<SVertex*>& GetVertices() const;
		void ReadIndices(IO::ZBinaryReader& binaryReader);
		void ReadVertexPosition(IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex, const bool isHighRes, const IOI::SVector4* scale, const IOI::SVector4* bias);
		void ReadVertexJointAndWeight(IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex);
		void ReadVertexNormal(IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex);
		void ReadVertexTangent(IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex);
		void ReadVertexBitangent(IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex);
		void ReadVertexUVs(IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex, const IOI::SVector2* scale, const IOI::SVector2* bias);
		void ReadVertexColor(IO::ZBinaryReader& binaryReader, const unsigned int vertexIndex);

	private:
		unsigned int indexCount;
		unsigned int vertexCount;
		std::vector<unsigned short> indices;
		std::vector<SVertex*> vertices;
	};
}
