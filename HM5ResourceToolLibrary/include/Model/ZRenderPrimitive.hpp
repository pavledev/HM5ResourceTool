#pragma once

#include <string>
#include <vector>
#include "ZMeshBufferBuilder.hpp"
#include "ZUtility.hpp"
#include "EPropertyFlags.hpp"
#include "Glacier2/Math/SVector2.hpp"
#include "Glacier2/Math/SVector4.hpp"

namespace HM5ResourceTool
{
	class ZRenderPrimitive
	{
	public:
		enum class ESubType
		{
			standard,
			linked,
			weighted,
			standardUV2,
			standardUV3,
			standardUV4
		};

		class ZObjectHeader
		{
		public:
			const ESubType GetSubType() const;
			const EPropertyFlags GetPropertyFlags() const;
			const unsigned char GetLODMask() const;
			const IOI::SVector3* GetBoundingBoxMin() const;
			const IOI::SVector3* GetBoundingBoxMax() const;

			void Read(IO::ZBinaryReader& binaryReader);

		private:
			struct SHeader
			{
				unsigned char lDrawDestination;
				unsigned char lPackType;
				unsigned short lType;
				unsigned char lSubType;
				unsigned char lProperties;
				unsigned char lLODMask;
				unsigned char lVariantId;
				unsigned char nZBias;
				unsigned char nZOffset;
				unsigned short lMaterialId;
				unsigned int lWireColor;
				unsigned int lColor1;
				IOI::SVector3 vBoundingBoxMin;
				IOI::SVector3 vBoundingBoxMax;
			};

			SHeader* header;
		};

		class ZObjectDetails
		{
		public:
			const unsigned int GetSubMeshTableOffset() const;
			const IOI::SVector4* GetMeshVertexPositionScale() const;
			const IOI::SVector4* GetMeshVertexPositionBias() const;
			const IOI::SVector2* GetMeshVertexUVScale() const;
			const IOI::SVector2* GetMeshVertexUVBias() const;
			void Read(IO::ZBinaryReader& binaryReader);

		private:
			struct SDetails
			{
				unsigned int lSubMeshTable;
				unsigned int lNumFrames;
				unsigned short lFrameStart;
				unsigned short lFrameStep;
				IOI::SVector4 vPosScale;
				IOI::SVector4 vPosBias;
				IOI::SVector2 vTexScale;
				IOI::SVector2 vTexBias;
			};

			SDetails* details;
		};

		class ZMeshDetails
		{
		public:
			const unsigned int GetVertexCount() const;
			const unsigned int GetVerticesOffset() const;
			const unsigned int GetIndexCount() const;
			const unsigned int GetIndicesOffset() const;
			const unsigned int GetUVChannelCount() const;
			void Read(IO::ZBinaryReader& binaryReader);

		private:
			struct SDetails
			{
				unsigned int lNumVertices;
				unsigned int lVertices;
				unsigned int lNumIndices;
				unsigned int lIndices;
				unsigned int lCollision;
				unsigned short lColorStreamSize;
				unsigned char lNumUVChannels;
				unsigned char lDummy1;
			};

			SDetails* details;
		};

		class ZMesh
		{
		public:
			ZMesh();
			~ZMesh();
			const ZObjectHeader* GetHeader() const;
			const ZMeshDetails* GetDetails() const;
			const std::vector<unsigned short>& GetIndices() const;
			const std::vector<SVertex*>& GetVertices() const;
			void Deserialize(IO::ZBinaryReader& binaryReader, const unsigned int meshOffset);
			void ReadBuffers(IO::ZBinaryReader& binaryReader, ZObjectHeader* objectHeader, ZObjectDetails* objectDetails);

		private:
			ZObjectHeader* header;
			ZMeshDetails* details;
			ZMeshBufferBuilder meshBufferBuilder;
		};

		class ZObject
		{
		public:
			ZObject();
			~ZObject();
			const ZObjectHeader* GetHeader() const;
			const ZObjectDetails* GetDetails() const;
			const ZMesh* GetMesh() const;
			void Deserialize(IO::ZBinaryReader& binaryReader, const unsigned int headerOffset);

		private:
			ZObjectHeader* header;
			ZObjectDetails* details;
			ZMesh* mesh;
		};

		class ZPrimaryHeader
		{
		public:
			const unsigned int GetObjectCount() const;
			const unsigned int GetObjectTableOffset() const;

		private:
			struct SHeader
			{
				unsigned char lDrawDestination;
				unsigned char lPackType;
				unsigned short lType;
				unsigned int lPropertyFlags;
				unsigned int lBoneRigResourceIndex;
				unsigned int lNumObjects;
				unsigned int lObjectTable;
				IOI::SVector3 vBoundingBoxMin;
				IOI::SVector3 vBoundingBoxMax;
			};

			SHeader header;
		};

		ZRenderPrimitive();
		~ZRenderPrimitive();
		const ZPrimaryHeader* GetPriaryHeader() const;
		const std::vector<ZObject*>& GetObjects() const;
		void Deserialize(const std::string& filePath);
		void Deserialize(const void* buffer, const unsigned int dataSize);
		void Deserialize(IO::ZBinaryReader& binaryReader);
		void ConvertToOBJ(const std::string& objFilePath);
		void ConvertToGLTF(const std::string& gltfFilePath);

	private:
		unsigned int primaryOffset;
		ZPrimaryHeader* primaryHeader;
		std::vector<ZObject*> objects;
	};
}
