#pragma once

#include <string>
#include <dxgiformat.h>
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DDS.h"
#include "./Render/ERenderFormat.hpp"
#include "./Render/ERenderResourceMipInterpolation.hpp"
#include "./Render/SRenderFormatDesc.hpp"
#include "IO/ZBinaryReader.hpp"
#include "IO/ZBinaryWriter.hpp"
#include "Texture/DDS.hpp"

namespace IOI
{
	class ZTextureMap
	{
	public:
		enum ETextureFlags
		{
			TEXF_SWIZZLED = 1,
			TEXF_DEFERRED = 2,
			TEXF_MEMORY_READY_XBOX360 = 4,
			TEXF_GAMMA = 8,
			TEXF_EMISSIVE = 16,
			TEXF_DDSC_ENCODED = 32
		};

		enum EInterpretAs
		{
			INTERPRET_AS_COLOR = 0,
			INTERPRET_AS_NORMAL = 1,
			INTERPRET_AS_HEIGHT = 2
		};

		enum EDimensions
		{
			DIMENSIONS_2D = 0,
			DIMENSIONS_CUBE = 1,
			DIMENSIONS_VOLUME = 2
		};

		struct SMipLevel
		{
			unsigned int nWidth;
			unsigned int nHeight;
			unsigned int nSizeInBytes;
			const unsigned char* pData;
		};

		struct STextureMapHeader
		{
			unsigned int nNumSlices;
			unsigned int nTotalSize;
			unsigned int nFlags;
			unsigned short nWidth;
			unsigned short nHeight;
			unsigned short nFormat;
			unsigned char nNumMipLevels;
			unsigned char nDefaultMipLevel;
			unsigned char nInterpretAs;
			unsigned char nDimensions;
			unsigned char nMipInterpolation;
			unsigned int nIADataSize;
			unsigned int nIADataOffset;
		};

		ZTextureMap(const std::string& texPath);
		ZTextureMap(const void* data, const unsigned int dataSize);
		~ZTextureMap();
		void Initialize(HM5ResourceTool::IO::ZBinaryReader& texBinaryReader);
		const STextureMapHeader* GetHeader();
		void GetMipLevel(unsigned int nMipLevel, SMipLevel& mipLevel);
		DirectX::TexMetadata* GetTexMetadata();
		DirectX::ScratchImage* GetScratchImage();
		static DXGI_FORMAT GetDXGIFormat(const ERenderFormat renderFormat);
		static ERenderFormat GetRenderFormat(const DXGI_FORMAT dxgiFormat);
		static DirectX::TEX_DIMENSION GetTexDimension(const ZTextureMap::EDimensions dimension);
		static void ZRenderGetFormatDesc(SRenderFormatDesc* pDesc, const ERenderFormat& eFormat);
		DirectX::TexMetadata GenerateTexMetadata();
		void ConvertR8G8ToB8G8R8A8();
		void ConvertB8G8R8A8ToR8G8();
		static STextureMapHeader GenerateTextureMapHeader(const STextureMapHeader& oldTextureMapHeader, const DirectX::ScratchImage& scratchImage);
		static HRESULT WriteToMemory(const STextureMapHeader& oldTextureMapHeader, const DirectX::ScratchImage& scratchImage, void** data, unsigned int& dataSize);
		static HRESULT WriteToFile(const STextureMapHeader& oldTextureMapHeader, const DirectX::ScratchImage& scratchImage, const std::string& texPath);
		static HRESULT Write(const STextureMapHeader& oldTextureMapHeader, const DirectX::ScratchImage& scratchImage, HM5ResourceTool::IO::ZBinaryWriter binaryWriter);

	private:
		STextureMapHeader textureMapHeader;
		unsigned char* data;
		DirectX::TexMetadata texMetadata;
		DirectX::ScratchImage scratchImage;
		HM5ResourceTool::IO::ZBinaryReader* texBinaryReader;
	};
}
