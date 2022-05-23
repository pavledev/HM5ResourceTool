#include "pch.h"
#include <stdexcept>
#include <DirectXTex/DirectXTexP.h>
#include "Glacier2/ZTextureMap.hpp"

namespace IOI
{
    ZTextureMap::ZTextureMap(const std::string& texPath)
    {
        texBinaryReader = new HM5ResourceTool::IO::ZBinaryReader(texPath);

        Initialize(*texBinaryReader);
    }

    ZTextureMap::ZTextureMap(const void* data, const unsigned int dataSize)
    {
        texBinaryReader = new HM5ResourceTool::IO::ZBinaryReader(data, dataSize);

        Initialize(*texBinaryReader);
    }

    ZTextureMap::~ZTextureMap()
    {
        delete texBinaryReader;
    }

    void ZTextureMap::Initialize(HM5ResourceTool::IO::ZBinaryReader& texBinaryReader)
    {
        textureMapHeader = *static_cast<STextureMapHeader*>(texBinaryReader.ReadBytes(sizeof(STextureMapHeader)));
        data = reinterpret_cast<unsigned char*>(texBinaryReader.GetInputStream()->GetCurrentPointer());
        texMetadata = GenerateTexMetadata();

        scratchImage.Initialize(texMetadata);

        memcpy(scratchImage.GetPixels(), data, scratchImage.GetPixelsSize());

        if (textureMapHeader.nFormat == RENDER_FORMAT_R8G8_UNORM)
        {
            ConvertR8G8ToB8G8R8A8();
        }
    }

    const ZTextureMap::STextureMapHeader* ZTextureMap::GetHeader()
    {
        return &textureMapHeader;
    }

    void ZTextureMap::GetMipLevel(unsigned int nMipLevel, SMipLevel& mipLevel)
    {
        SRenderFormatDesc pDesc;
        ERenderFormat eFormat = static_cast<ERenderFormat>(textureMapHeader.nFormat);

        mipLevel.nWidth = textureMapHeader.nWidth;
        mipLevel.nHeight = textureMapHeader.nHeight;

        ZRenderGetFormatDesc(&pDesc, eFormat);

        mipLevel.nSizeInBytes = pDesc.nBytesPerBlock *
            ((pDesc.nBlockWidth + mipLevel.nWidth - 1) /
                pDesc.nBlockWidth *
                ((pDesc.nBlockHeight + mipLevel.nHeight - 1) /
                    pDesc.nBlockHeight));

        unsigned int offset = 0;

        if (nMipLevel)
        {
            int i = 0;

            do
            {
                unsigned int width = mipLevel.nWidth + 1;
                unsigned int height = mipLevel.nHeight + 1;

                offset += mipLevel.nSizeInBytes;
                width >>= 1;
                height >>= 1;
                mipLevel.nWidth = width;
                mipLevel.nHeight = height;

                ZRenderGetFormatDesc(&pDesc, eFormat);

                i = nMipLevel-- == 1;

                mipLevel.nSizeInBytes = pDesc.nBytesPerBlock *
                    ((pDesc.nBlockWidth + width - 1) /
                        pDesc.nBlockWidth *
                        ((pDesc.nBlockHeight + height - 1) /
                            pDesc.nBlockHeight));
            }
            while (!i);
        }

        mipLevel.pData = &data[offset];
    }

    DirectX::TexMetadata* ZTextureMap::GetTexMetadata()
    {
        return &texMetadata;
    }

    DirectX::ScratchImage* ZTextureMap::GetScratchImage()
    {
        return &scratchImage;
    }

    DXGI_FORMAT ZTextureMap::GetDXGIFormat(const ERenderFormat renderFormat)
    {
        DXGI_FORMAT result;

        switch (renderFormat)
        {
        case RENDER_FORMAT_R32G32B32A32_TYPELESS:
            result = DXGI_FORMAT_R32G32B32A32_TYPELESS;

            break;
        case RENDER_FORMAT_R32G32B32A32_FLOAT:
            result = DXGI_FORMAT_R32G32B32A32_FLOAT;

            break;
        case RENDER_FORMAT_R32G32B32A32_UINT:
            result = DXGI_FORMAT_R32G32B32A32_UINT;

            break;
        case RENDER_FORMAT_R32G32B32A32_SINT:
            result = DXGI_FORMAT_R32G32B32A32_SINT;

            break;
        case RENDER_FORMAT_R32G32B32_TYPELESS:
            result = DXGI_FORMAT_R32G32B32_TYPELESS;

            break;
        case RENDER_FORMAT_R32G32B32_FLOAT:
            result = DXGI_FORMAT_R32G32B32_FLOAT;

            break;
        case RENDER_FORMAT_R32G32B32_UINT:
            result = DXGI_FORMAT_R32G32B32_UINT;

            break;
        case RENDER_FORMAT_R32G32B32_SINT:
            result = DXGI_FORMAT_R32G32B32_SINT;

            break;
        case RENDER_FORMAT_R16G16B16A16_TYPELESS:
            result = DXGI_FORMAT_R16G16B16A16_TYPELESS;

            break;
        case RENDER_FORMAT_R16G16B16A16_FLOAT:
            result = DXGI_FORMAT_R16G16B16A16_FLOAT;

            break;
        case RENDER_FORMAT_R16G16B16A16_UNORM:
            result = DXGI_FORMAT_R16G16B16A16_UNORM;

            break;
        case RENDER_FORMAT_R16G16B16A16_UINT:
            result = DXGI_FORMAT_R16G16B16A16_UINT;

            break;
        case RENDER_FORMAT_R16G16B16A16_SNORM:
            result = DXGI_FORMAT_R16G16B16A16_SNORM;

            break;
        case RENDER_FORMAT_R16G16B16A16_SINT:
            result = DXGI_FORMAT_R16G16B16A16_SINT;

            break;
        case RENDER_FORMAT_R32G32_TYPELESS:
            result = DXGI_FORMAT_R32G32_TYPELESS;

            break;
        case RENDER_FORMAT_R32G32_FLOAT:
            result = DXGI_FORMAT_R32G32_FLOAT;

            break;
        case RENDER_FORMAT_R32G32_UINT:
            result = DXGI_FORMAT_R32G32_UINT;

            break;
        case RENDER_FORMAT_R32G32_SINT:
            result = DXGI_FORMAT_R32G32_SINT;

            break;
        case RENDER_FORMAT_R32G8X24_TYPELESS:
            result = DXGI_FORMAT_R32G8X24_TYPELESS;

            break;
        case RENDER_FORMAT_D32_FLOAT_S8X24_UINT:
            result = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

            break;
        case RENDER_FORMAT_R32_FLOAT_X8X24_TYPELESS:
            result = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;

            break;
        case RENDER_FORMAT_X32_TYPELESS_G8X24_UINT:
            result = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

            break;
        case RENDER_FORMAT_R10G10B10A2_TYPELESS:
            result = DXGI_FORMAT_R10G10B10A2_TYPELESS;

            break;
        case RENDER_FORMAT_R10G10B10A2_UNORM:
            result = DXGI_FORMAT_R10G10B10A2_UNORM;

            break;
        case RENDER_FORMAT_R10G10B10A2_UINT:
            result = DXGI_FORMAT_R10G10B10A2_UINT;

            break;
        case RENDER_FORMAT_R11G11B10_FLOAT:
            result = DXGI_FORMAT_R11G11B10_FLOAT;

            break;
        case RENDER_FORMAT_R8G8B8A8_TYPELESS:
            result = DXGI_FORMAT_R8G8B8A8_TYPELESS;

            break;
        case RENDER_FORMAT_R8G8B8A8_UNORM:
            result = DXGI_FORMAT_R8G8B8A8_UNORM;

            break;
        case RENDER_FORMAT_R8G8B8A8_UNORM_SRGB:
            result = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

            break;
        case RENDER_FORMAT_R8G8B8A8_UINT:
            result = DXGI_FORMAT_R8G8B8A8_UINT;

            break;
        case RENDER_FORMAT_R8G8B8A8_SNORM:
            result = DXGI_FORMAT_R8G8B8A8_SNORM;

            break;
        case RENDER_FORMAT_R8G8B8A8_SINT:
            result = DXGI_FORMAT_R8G8B8A8_SINT;

            break;
        case RENDER_FORMAT_R16G16_TYPELESS:
            result = DXGI_FORMAT_R16G16_TYPELESS;

            break;
        case RENDER_FORMAT_R16G16_FLOAT:
            result = DXGI_FORMAT_R16G16_FLOAT;

            break;
        case RENDER_FORMAT_R16G16_UNORM:
            result = DXGI_FORMAT_R16G16_UNORM;

            break;
        case RENDER_FORMAT_R16G16_UINT:
            result = DXGI_FORMAT_R16G16_UINT;

            break;
        case RENDER_FORMAT_R16G16_SNORM:
            result = DXGI_FORMAT_R16G16_SNORM;

            break;
        case RENDER_FORMAT_R16G16_SINT:
            result = DXGI_FORMAT_R16G16_SINT;

            break;
        case RENDER_FORMAT_R32_TYPELESS:
            result = DXGI_FORMAT_R32_TYPELESS;

            break;
        case RENDER_FORMAT_D32_FLOAT:
            result = DXGI_FORMAT_D32_FLOAT;

            break;
        case RENDER_FORMAT_R32_FLOAT:
            result = DXGI_FORMAT_R32_FLOAT;

            break;
        case RENDER_FORMAT_R32_UINT:
        case RENDER_FORMAT_INDEX_32:
            result = DXGI_FORMAT_R32_UINT;

            break;
        case RENDER_FORMAT_R32_SINT:
            result = DXGI_FORMAT_R32_SINT;

            break;
        case RENDER_FORMAT_R24G8_TYPELESS:
            result = DXGI_FORMAT_R24G8_TYPELESS;

            break;
        case RENDER_FORMAT_D24_UNORM_S8_UINT:
            result = DXGI_FORMAT_D24_UNORM_S8_UINT;

            break;
        case RENDER_FORMAT_R24_UNORM_X8_TYPELESS:
            result = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

            break;
        case RENDER_FORMAT_X24_TYPELESS_G8_UINT:
            result = DXGI_FORMAT_X24_TYPELESS_G8_UINT;

            break;
        case RENDER_FORMAT_R9G9B9E5_SHAREDEXP:
            result = DXGI_FORMAT_R9G9B9E5_SHAREDEXP;

            break;
        case RENDER_FORMAT_R8G8_B8G8_UNORM:
            result = DXGI_FORMAT_R8G8_B8G8_UNORM;

            break;
        case RENDER_FORMAT_G8R8_G8B8_UNORM:
            result = DXGI_FORMAT_G8R8_G8B8_UNORM;

            break;
        case RENDER_FORMAT_R8G8_TYPELESS:
            result = DXGI_FORMAT_R8G8_TYPELESS;

            break;
        case RENDER_FORMAT_R8G8_UNORM:
            result = DXGI_FORMAT_R8G8_UNORM;

            break;
        case RENDER_FORMAT_R8G8_UINT:
            result = DXGI_FORMAT_R8G8_UINT;

            break;
        case RENDER_FORMAT_R8G8_SNORM:
            result = DXGI_FORMAT_R8G8_SNORM;

            break;
        case RENDER_FORMAT_R8G8_SINT:
            result = DXGI_FORMAT_R8G8_SINT;

            break;
        case RENDER_FORMAT_R16_TYPELESS:
            result = DXGI_FORMAT_R16_TYPELESS;

            break;
        case RENDER_FORMAT_R16_FLOAT:
            result = DXGI_FORMAT_R16_FLOAT;

            break;
        case RENDER_FORMAT_D16_UNORM:
            result = DXGI_FORMAT_D16_UNORM;

            break;
        case RENDER_FORMAT_R16_UNORM:
            result = DXGI_FORMAT_R16_UNORM;

            break;
        case RENDER_FORMAT_R16_UINT:
        case RENDER_FORMAT_INDEX_16:
            result = DXGI_FORMAT_R16_UINT;

            break;
        case RENDER_FORMAT_R16_SNORM:
            result = DXGI_FORMAT_R16_SNORM;

            break;
        case RENDER_FORMAT_R16_SINT:
            result = DXGI_FORMAT_R16_SINT;

            break;
        case RENDER_FORMAT_B5G6R5_UNORM:
            result = DXGI_FORMAT_B5G6R5_UNORM;

            break;
        case RENDER_FORMAT_B5G5R5A1_UNORM:
            result = DXGI_FORMAT_B5G5R5A1_UNORM;

            break;
        case RENDER_FORMAT_R8_TYPELESS:
            result = DXGI_FORMAT_R8_TYPELESS;

            break;
        case RENDER_FORMAT_R8_UNORM:
            result = DXGI_FORMAT_R8_UNORM;

            break;
        case RENDER_FORMAT_R8_UINT:
            result = DXGI_FORMAT_R8_UINT;

            break;
        case RENDER_FORMAT_R8_SNORM:
            result = DXGI_FORMAT_R8_SNORM;

            break;
        case RENDER_FORMAT_R8_SINT:
            result = DXGI_FORMAT_R8_SINT;

            break;
        case RENDER_FORMAT_A8_UNORM:
            result = DXGI_FORMAT_A8_UNORM;

            break;
        case RENDER_FORMAT_R1_UNORM:
            result = DXGI_FORMAT_R1_UNORM;

            break;
        case RENDER_FORMAT_BC1_TYPELESS:
            result = DXGI_FORMAT_BC1_TYPELESS;

            break;
        case RENDER_FORMAT_BC1_UNORM:
            result = DXGI_FORMAT_BC1_UNORM;

            break;
        case RENDER_FORMAT_BC1_UNORM_SRGB:
            result = DXGI_FORMAT_BC1_UNORM_SRGB;

            break;
        case RENDER_FORMAT_BC2_TYPELESS:
            result = DXGI_FORMAT_BC2_TYPELESS;

            break;
        case RENDER_FORMAT_BC2_UNORM:
            result = DXGI_FORMAT_BC2_UNORM;

            break;
        case RENDER_FORMAT_BC2_UNORM_SRGB:
            result = DXGI_FORMAT_BC2_UNORM_SRGB;

            break;
        case RENDER_FORMAT_BC3_TYPELESS:
            result = DXGI_FORMAT_BC3_TYPELESS;

            break;
        case RENDER_FORMAT_BC3_UNORM:
            result = DXGI_FORMAT_BC3_UNORM;

            break;
        case RENDER_FORMAT_BC3_UNORM_SRGB:
            result = DXGI_FORMAT_BC3_UNORM_SRGB;

            break;
        case RENDER_FORMAT_BC4_TYPELESS:
            result = DXGI_FORMAT_BC4_TYPELESS;

            break;
        case RENDER_FORMAT_BC4_UNORM:
            result = DXGI_FORMAT_BC4_UNORM;

            break;
        case RENDER_FORMAT_BC4_SNORM:
            result = DXGI_FORMAT_BC4_SNORM;

            break;
        case RENDER_FORMAT_BC5_TYPELESS:
            result = DXGI_FORMAT_BC5_TYPELESS;

            break;
        case RENDER_FORMAT_BC5_UNORM:
            result = DXGI_FORMAT_BC5_UNORM;

            break;
        case RENDER_FORMAT_BC5_SNORM:
            result = DXGI_FORMAT_BC5_SNORM;

            break;
        case RENDER_FORMAT_LE_X2R10G10B10_UNORM:
            throw std::invalid_argument("LE_X2R10G10B10 texture type isn't supported.");
        case RENDER_FORMAT_LE_X8R8G8B8_UNORM:
            throw std::invalid_argument("LE_X8R8G8B8 texture type isn't supported.");
        case RENDER_FORMAT_X16Y16Z16_SNORM:
            throw std::invalid_argument("X16Y16Z16 texture type isn't supported.");
        }

        return result;
    }

    ERenderFormat ZTextureMap::GetRenderFormat(const DXGI_FORMAT dxgiFormat)
    {
        ERenderFormat result;

        switch (dxgiFormat)
        {
        case DXGI_FORMAT_R32G32B32A32_TYPELESS:
            result = RENDER_FORMAT_R32G32B32A32_TYPELESS;

            break;
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
            result = RENDER_FORMAT_R32G32B32A32_FLOAT;

            break;
        case DXGI_FORMAT_R32G32B32A32_UINT:
            result = RENDER_FORMAT_R32G32B32A32_UINT;

            break;
        case DXGI_FORMAT_R32G32B32A32_SINT:
            result = RENDER_FORMAT_R32G32B32A32_SINT;

            break;
        case DXGI_FORMAT_R32G32B32_TYPELESS:
            result = RENDER_FORMAT_R32G32B32_TYPELESS;

            break;
        case DXGI_FORMAT_R32G32B32_FLOAT:
            result = RENDER_FORMAT_R32G32B32_FLOAT;

            break;
        case DXGI_FORMAT_R32G32B32_UINT:
            result = RENDER_FORMAT_R32G32B32_UINT;

            break;
        case DXGI_FORMAT_R32G32B32_SINT:
            result = RENDER_FORMAT_R32G32B32_SINT;

            break;
        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
            result = RENDER_FORMAT_R16G16B16A16_TYPELESS;

            break;
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
            result = RENDER_FORMAT_R16G16B16A16_FLOAT;

            break;
        case DXGI_FORMAT_R16G16B16A16_UNORM:
            result = RENDER_FORMAT_R16G16B16A16_UNORM;

            break;
        case DXGI_FORMAT_R16G16B16A16_UINT:
            result = RENDER_FORMAT_R16G16B16A16_UINT;

            break;
        case DXGI_FORMAT_R16G16B16A16_SNORM:
            result = RENDER_FORMAT_R16G16B16A16_SNORM;

            break;
        case DXGI_FORMAT_R16G16B16A16_SINT:
            result = RENDER_FORMAT_R16G16B16A16_SINT;

            break;
        case DXGI_FORMAT_R32G32_TYPELESS:
            result = RENDER_FORMAT_R32G32_TYPELESS;

            break;
        case DXGI_FORMAT_R32G32_FLOAT:
            result = RENDER_FORMAT_R32G32_FLOAT;

            break;
        case DXGI_FORMAT_R32G32_UINT:
            result = RENDER_FORMAT_R32G32_UINT;

            break;
        case DXGI_FORMAT_R32G32_SINT:
            result = RENDER_FORMAT_R32G32_SINT;

            break;
        case DXGI_FORMAT_R32G8X24_TYPELESS:
            result = RENDER_FORMAT_R32G8X24_TYPELESS;

            break;
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
            result = RENDER_FORMAT_D32_FLOAT_S8X24_UINT;

            break;
        case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
            result = RENDER_FORMAT_R32_FLOAT_X8X24_TYPELESS;

            break;
        case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
            result = RENDER_FORMAT_X32_TYPELESS_G8X24_UINT;

            break;
        case DXGI_FORMAT_R10G10B10A2_TYPELESS:
            result = RENDER_FORMAT_R10G10B10A2_TYPELESS;

            break;
        case DXGI_FORMAT_R10G10B10A2_UNORM:
            result = RENDER_FORMAT_R10G10B10A2_UNORM;

            break;
        case DXGI_FORMAT_R10G10B10A2_UINT:
            result = RENDER_FORMAT_R10G10B10A2_UINT;

            break;
        case DXGI_FORMAT_R11G11B10_FLOAT:
            result = RENDER_FORMAT_R11G11B10_FLOAT;

            break;
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
            result = RENDER_FORMAT_R8G8B8A8_TYPELESS;

            break;
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            result = RENDER_FORMAT_R8G8B8A8_UNORM;

            break;
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            result = RENDER_FORMAT_R8G8B8A8_UNORM_SRGB;

            break;
        case DXGI_FORMAT_R8G8B8A8_UINT:
            result = RENDER_FORMAT_R8G8B8A8_UINT;

            break;
        case DXGI_FORMAT_R8G8B8A8_SNORM:
            result = RENDER_FORMAT_R8G8B8A8_SNORM;

            break;
        case DXGI_FORMAT_R8G8B8A8_SINT:
            result = RENDER_FORMAT_R8G8B8A8_SINT;

            break;
        case DXGI_FORMAT_R16G16_TYPELESS:
            result = RENDER_FORMAT_R16G16_TYPELESS;

            break;
        case DXGI_FORMAT_R16G16_FLOAT:
            result = RENDER_FORMAT_R16G16_FLOAT;

            break;
        case DXGI_FORMAT_R16G16_UNORM:
            result = RENDER_FORMAT_R16G16_UNORM;

            break;
        case DXGI_FORMAT_R16G16_UINT:
            result = RENDER_FORMAT_R16G16_UINT;

            break;
        case DXGI_FORMAT_R16G16_SNORM:
            result = RENDER_FORMAT_R16G16_SNORM;

            break;
        case DXGI_FORMAT_R16G16_SINT:
            result = RENDER_FORMAT_R16G16_SINT;

            break;
        case DXGI_FORMAT_R32_TYPELESS:
            result = RENDER_FORMAT_R32_TYPELESS;

            break;
        case DXGI_FORMAT_D32_FLOAT:
            result = RENDER_FORMAT_D32_FLOAT;

            break;
        case DXGI_FORMAT_R32_FLOAT:
            result = RENDER_FORMAT_R32_FLOAT;

            break;
        case DXGI_FORMAT_R32_UINT:
            result = RENDER_FORMAT_R32_UINT;

            break;
        case DXGI_FORMAT_R32_SINT:
            result = RENDER_FORMAT_R32_SINT;

            break;
        case DXGI_FORMAT_R24G8_TYPELESS:
            result = RENDER_FORMAT_R24G8_TYPELESS;

            break;
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
            result = RENDER_FORMAT_D24_UNORM_S8_UINT;

            break;
        case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
            result = RENDER_FORMAT_R24_UNORM_X8_TYPELESS;

            break;
        case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
            result = RENDER_FORMAT_X24_TYPELESS_G8_UINT;

            break;
        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
            result = RENDER_FORMAT_R9G9B9E5_SHAREDEXP;

            break;
        case DXGI_FORMAT_R8G8_B8G8_UNORM:
            result = RENDER_FORMAT_R8G8_B8G8_UNORM;

            break;
        case DXGI_FORMAT_G8R8_G8B8_UNORM:
            result = RENDER_FORMAT_G8R8_G8B8_UNORM;

            break;
        case DXGI_FORMAT_R8G8_TYPELESS:
            result = RENDER_FORMAT_R8G8_TYPELESS;

            break;
        case DXGI_FORMAT_R8G8_UNORM:
            result = RENDER_FORMAT_R8G8_UNORM;

            break;
        case DXGI_FORMAT_R8G8_UINT:
            result = RENDER_FORMAT_R8G8_UINT;

            break;
        case DXGI_FORMAT_R8G8_SNORM:
            result = RENDER_FORMAT_R8G8_SNORM;

            break;
        case DXGI_FORMAT_R8G8_SINT:
            result = RENDER_FORMAT_R8G8_SINT;

            break;
        case DXGI_FORMAT_R16_TYPELESS:
            result = RENDER_FORMAT_R16_TYPELESS;

            break;
        case DXGI_FORMAT_R16_FLOAT:
            result = RENDER_FORMAT_R16_FLOAT;

            break;
        case DXGI_FORMAT_D16_UNORM:
            result = RENDER_FORMAT_D16_UNORM;

            break;
        case DXGI_FORMAT_R16_UNORM:
            result = RENDER_FORMAT_R16_UNORM;

            break;
        case DXGI_FORMAT_R16_UINT:
            result = RENDER_FORMAT_R16_UINT;

            break;
        case DXGI_FORMAT_R16_SNORM:
            result = RENDER_FORMAT_R16_SNORM;

            break;
        case DXGI_FORMAT_R16_SINT:
            result = RENDER_FORMAT_R16_SINT;

            break;
        case DXGI_FORMAT_B5G6R5_UNORM:
            result = RENDER_FORMAT_B5G6R5_UNORM;

            break;
        case DXGI_FORMAT_B5G5R5A1_UNORM:
            result = RENDER_FORMAT_B5G5R5A1_UNORM;

            break;
        case DXGI_FORMAT_R8_TYPELESS:
            result = RENDER_FORMAT_R8_TYPELESS;

            break;
        case DXGI_FORMAT_R8_UNORM:
            result = RENDER_FORMAT_R8_UNORM;

            break;
        case DXGI_FORMAT_R8_UINT:
            result = RENDER_FORMAT_R8_UINT;

            break;
        case DXGI_FORMAT_R8_SNORM:
            result = RENDER_FORMAT_R8_SNORM;

            break;
        case DXGI_FORMAT_R8_SINT:
            result = RENDER_FORMAT_R8_SINT;

            break;
        case DXGI_FORMAT_A8_UNORM:
            result = RENDER_FORMAT_A8_UNORM;

            break;
        case DXGI_FORMAT_R1_UNORM:
            result = RENDER_FORMAT_R1_UNORM;

            break;
        case DXGI_FORMAT_BC1_TYPELESS:
            result = RENDER_FORMAT_BC1_TYPELESS;

            break;
        case DXGI_FORMAT_BC1_UNORM:
            result = RENDER_FORMAT_BC1_UNORM;

            break;
        case DXGI_FORMAT_BC1_UNORM_SRGB:
            result = RENDER_FORMAT_BC1_UNORM_SRGB;

            break;
        case DXGI_FORMAT_BC2_TYPELESS:
            result = RENDER_FORMAT_BC2_TYPELESS;

            break;
        case DXGI_FORMAT_BC2_UNORM:
            result = RENDER_FORMAT_BC2_UNORM;

            break;
        case DXGI_FORMAT_BC2_UNORM_SRGB:
            result = RENDER_FORMAT_BC2_UNORM_SRGB;

            break;
        case DXGI_FORMAT_BC3_TYPELESS:
            result = RENDER_FORMAT_BC3_TYPELESS;

            break;
        case DXGI_FORMAT_BC3_UNORM:
            result = RENDER_FORMAT_BC3_UNORM;

            break;
        case DXGI_FORMAT_BC3_UNORM_SRGB:
            result = RENDER_FORMAT_BC3_UNORM_SRGB;

            break;
        case DXGI_FORMAT_BC4_TYPELESS:
            result = RENDER_FORMAT_BC4_TYPELESS;

            break;
        case DXGI_FORMAT_BC4_UNORM:
            result = RENDER_FORMAT_BC4_UNORM;

            break;
        case DXGI_FORMAT_BC4_SNORM:
            result = RENDER_FORMAT_BC4_SNORM;

            break;
        case DXGI_FORMAT_BC5_TYPELESS:
            result = RENDER_FORMAT_BC5_TYPELESS;

            break;
        case DXGI_FORMAT_BC5_UNORM:
            result = RENDER_FORMAT_BC5_UNORM;

            break;
        case DXGI_FORMAT_BC5_SNORM:
            result = RENDER_FORMAT_BC5_SNORM;

            break;
        }

        return result;
    }

    DirectX::TEX_DIMENSION ZTextureMap::GetTexDimension(const ZTextureMap::EDimensions dimension)
    {
        DirectX::TEX_DIMENSION result;

        switch (dimension)
        {
        case ZTextureMap::EDimensions::DIMENSIONS_2D:
        case ZTextureMap::EDimensions::DIMENSIONS_CUBE:
            result = DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE2D;

            break;
        case ZTextureMap::EDimensions::DIMENSIONS_VOLUME:
            result = DirectX::TEX_DIMENSION::TEX_DIMENSION_TEXTURE3D;

            break;
        }

        return result;
    }

    void ZTextureMap::ZRenderGetFormatDesc(SRenderFormatDesc* pDesc, const ERenderFormat& eFormat)
    {
        switch (eFormat)
        {
        case RENDER_FORMAT_R32G32B32A32_TYPELESS:
        case RENDER_FORMAT_R32G32B32A32_FLOAT:
        case RENDER_FORMAT_R32G32B32A32_UINT:
        case RENDER_FORMAT_R32G32B32A32_SINT:
            pDesc->nBlockWidth = 1;
            pDesc->nBlockHeight = 1;
            pDesc->nBytesPerBlock = 16;
            break;
        case RENDER_FORMAT_R32G32B32_TYPELESS:
        case RENDER_FORMAT_R32G32B32_FLOAT:
        case RENDER_FORMAT_R32G32B32_UINT:
        case RENDER_FORMAT_R32G32B32_SINT:
            pDesc->nBlockWidth = 1;
            pDesc->nBlockHeight = 1;
            pDesc->nBytesPerBlock = 12;
            break;
        case RENDER_FORMAT_R16G16B16A16_TYPELESS:
        case RENDER_FORMAT_R16G16B16A16_FLOAT:
        case RENDER_FORMAT_R16G16B16A16_UNORM:
        case RENDER_FORMAT_R16G16B16A16_UINT:
        case RENDER_FORMAT_R16G16B16A16_SNORM:
        case RENDER_FORMAT_R16G16B16A16_SINT:
        case RENDER_FORMAT_R32G32_TYPELESS:
        case RENDER_FORMAT_R32G32_FLOAT:
        case RENDER_FORMAT_R32G32_UINT:
        case RENDER_FORMAT_R32G32_SINT:
        case RENDER_FORMAT_R32G8X24_TYPELESS:
        case RENDER_FORMAT_D32_FLOAT_S8X24_UINT:
        case RENDER_FORMAT_R32_FLOAT_X8X24_TYPELESS:
        case RENDER_FORMAT_X32_TYPELESS_G8X24_UINT:
            pDesc->nBlockWidth = 1;
            pDesc->nBlockHeight = 1;
            pDesc->nBytesPerBlock = 8;
            break;
        case RENDER_FORMAT_R10G10B10A2_TYPELESS:
        case RENDER_FORMAT_R10G10B10A2_UNORM:
        case RENDER_FORMAT_R10G10B10A2_UINT:
        case RENDER_FORMAT_R11G11B10_FLOAT:
        case RENDER_FORMAT_R8G8B8A8_TYPELESS:
        case RENDER_FORMAT_R8G8B8A8_UNORM:
        case RENDER_FORMAT_R8G8B8A8_UNORM_SRGB:
        case RENDER_FORMAT_R8G8B8A8_UINT:
        case RENDER_FORMAT_R8G8B8A8_SNORM:
        case RENDER_FORMAT_R8G8B8A8_SINT:
        case RENDER_FORMAT_R16G16_TYPELESS:
        case RENDER_FORMAT_R16G16_FLOAT:
        case RENDER_FORMAT_R16G16_UNORM:
        case RENDER_FORMAT_R16G16_UINT:
        case RENDER_FORMAT_R16G16_SNORM:
        case RENDER_FORMAT_R16G16_SINT:
        case RENDER_FORMAT_R32_TYPELESS:
        case RENDER_FORMAT_D32_FLOAT:
        case RENDER_FORMAT_R32_FLOAT:
        case RENDER_FORMAT_R32_UINT:
        case RENDER_FORMAT_R32_SINT:
        case RENDER_FORMAT_R24G8_TYPELESS:
        case RENDER_FORMAT_D24_UNORM_S8_UINT:
        case RENDER_FORMAT_R24_UNORM_X8_TYPELESS:
        case RENDER_FORMAT_X24_TYPELESS_G8_UINT:
        case RENDER_FORMAT_R9G9B9E5_SHAREDEXP:
        case RENDER_FORMAT_R8G8_B8G8_UNORM:
        case RENDER_FORMAT_G8R8_G8B8_UNORM:
        case RENDER_FORMAT_INDEX_32:
        case RENDER_FORMAT_LE_X8R8G8B8_UNORM:
            pDesc->nBlockWidth = 1;
            pDesc->nBlockHeight = 1;
            pDesc->nBytesPerBlock = 4;
            break;
        case RENDER_FORMAT_R8G8_TYPELESS:
        case RENDER_FORMAT_R8G8_UNORM:
        case RENDER_FORMAT_R8G8_UINT:
        case RENDER_FORMAT_R8G8_SNORM:
        case RENDER_FORMAT_R8G8_SINT:
        case RENDER_FORMAT_R16_TYPELESS:
        case RENDER_FORMAT_R16_FLOAT:
        case RENDER_FORMAT_D16_UNORM:
        case RENDER_FORMAT_R16_UNORM:
        case RENDER_FORMAT_R16_UINT:
        case RENDER_FORMAT_R16_SNORM:
        case RENDER_FORMAT_R16_SINT:
        case RENDER_FORMAT_B5G6R5_UNORM:
        case RENDER_FORMAT_B5G5R5A1_UNORM:
        case RENDER_FORMAT_INDEX_16:
            pDesc->nBlockWidth = 1;
            pDesc->nBlockHeight = 1;
            pDesc->nBytesPerBlock = 2;
            break;
        case RENDER_FORMAT_R8_TYPELESS:
        case RENDER_FORMAT_R8_UNORM:
        case RENDER_FORMAT_R8_UINT:
        case RENDER_FORMAT_R8_SNORM:
        case RENDER_FORMAT_R8_SINT:
        case RENDER_FORMAT_A8_UNORM:
            pDesc->nBlockWidth = 1;
            pDesc->nBlockHeight = 1;
            pDesc->nBytesPerBlock = 1;
            break;
        case RENDER_FORMAT_R1_UNORM:
            pDesc->nBlockWidth = 1;
            pDesc->nBlockHeight = 1;
            pDesc->nBytesPerBlock = 0;
            break;
        case RENDER_FORMAT_BC1_TYPELESS:
        case RENDER_FORMAT_BC1_UNORM:
        case RENDER_FORMAT_BC1_UNORM_SRGB:
        case RENDER_FORMAT_BC4_TYPELESS:
        case RENDER_FORMAT_BC4_UNORM:
        case RENDER_FORMAT_BC4_SNORM:
            pDesc->nBlockWidth = 4;
            pDesc->nBlockHeight = 4;
            pDesc->nBytesPerBlock = 8;
            break;
        case RENDER_FORMAT_BC2_TYPELESS:
        case RENDER_FORMAT_BC2_UNORM:
        case RENDER_FORMAT_BC2_UNORM_SRGB:
        case RENDER_FORMAT_BC3_TYPELESS:
        case RENDER_FORMAT_BC3_UNORM:
        case RENDER_FORMAT_BC3_UNORM_SRGB:
        case RENDER_FORMAT_BC5_TYPELESS:
        case RENDER_FORMAT_BC5_UNORM:
        case RENDER_FORMAT_BC5_SNORM:
            pDesc->nBlockWidth = 4;
            pDesc->nBlockHeight = 4;
            pDesc->nBytesPerBlock = 16;
            break;
        }
    }

    DirectX::TexMetadata ZTextureMap::GenerateTexMetadata()
    {
        unsigned int width = textureMapHeader.nWidth;
        unsigned int height = textureMapHeader.nHeight;
        bool isCubeMap = textureMapHeader.nDimensions == ZTextureMap::EDimensions::DIMENSIONS_CUBE;
        unsigned int arraySize = isCubeMap ? 6 : 1;
        int mipMapLevels = textureMapHeader.nNumMipLevels;
        unsigned int miscFlags = isCubeMap ? DirectX::TEX_MISC_FLAG::TEX_MISC_TEXTURECUBE : 0;
        const DXGI_FORMAT dxgiFormat = GetDXGIFormat(static_cast<ERenderFormat>(textureMapHeader.nFormat));
        DirectX::TEX_DIMENSION dimension = GetTexDimension(static_cast<EDimensions>(textureMapHeader.nDimensions));

        return DirectX::TexMetadata(width, height, 1, arraySize, mipMapLevels, miscFlags, 0, dxgiFormat, dimension);
    }

    void ZTextureMap::ConvertR8G8ToB8G8R8A8()
    {
        void* data = nullptr;
        unsigned int currentSize = 0;

        for (int level = 0; level < textureMapHeader.nNumMipLevels; ++level)
        {
            ZTextureMap::SMipLevel mipLevel;

            GetMipLevel(level, mipLevel);

            HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(mipLevel.pData, mipLevel.nSizeInBytes);
            HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(mipLevel.nSizeInBytes * 2);

            for (unsigned int i = 0; i < mipLevel.nSizeInBytes / 2; ++i)
            {
                unsigned char red = binaryReader.ReadByte();
                unsigned char green = binaryReader.ReadByte();

                binaryWriter.WriteByte(255);
                binaryWriter.WriteByte(green);
                binaryWriter.WriteByte(red);
                binaryWriter.WriteByte(255);
            }

            if (data)
            {
                data = realloc(data, currentSize + binaryWriter.GetOutputStream()->GetPosition());
            }
            else
            {
                data = malloc(binaryWriter.GetOutputStream()->GetPosition());
            }

            uintptr_t pointer = reinterpret_cast<uintptr_t>(data) + currentSize;

            memcpy(reinterpret_cast<void*>(pointer), binaryWriter.GetOutputStream()->GetBuffer(), binaryWriter.GetOutputStream()->GetPosition());
            currentSize += binaryWriter.GetOutputStream()->GetPosition();
        }

        texMetadata.format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
        this->data = reinterpret_cast<unsigned char*>(data);
    }

    void ZTextureMap::ConvertB8G8R8A8ToR8G8()
    {
        void* data = nullptr;
        unsigned int currentSize = 0;

        for (int level = 0; level < textureMapHeader.nNumMipLevels; ++level)
        {
            ZTextureMap::SMipLevel mipLevel;

            GetMipLevel(level, mipLevel);

            HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(mipLevel.pData, mipLevel.nSizeInBytes);
            HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(mipLevel.nSizeInBytes * 2);

            for (unsigned int i = 0; i < mipLevel.nSizeInBytes / 4; ++i)
            {
                unsigned char blue = binaryReader.ReadByte();
                unsigned char green = binaryReader.ReadByte();
                unsigned char red = binaryReader.ReadByte();
                unsigned char alpha = binaryReader.ReadByte();

                binaryWriter.WriteByte(red);
                binaryWriter.WriteByte(green);
            }

            if (data)
            {
                data = realloc(data, currentSize + binaryWriter.GetOutputStream()->GetPosition());
            }
            else
            {
                data = malloc(binaryWriter.GetOutputStream()->GetPosition());
            }

            uintptr_t pointer = reinterpret_cast<uintptr_t>(data) + currentSize;

            memcpy(reinterpret_cast<void*>(pointer), binaryWriter.GetOutputStream()->GetBuffer(), binaryWriter.GetOutputStream()->GetPosition());
            currentSize += binaryWriter.GetOutputStream()->GetPosition();
        }

        texMetadata.format = DXGI_FORMAT::DXGI_FORMAT_R8G8_UNORM;
        this->data = reinterpret_cast<unsigned char*>(data);
    }

    ZTextureMap::STextureMapHeader ZTextureMap::GenerateTextureMapHeader(const STextureMapHeader& oldTextureMapHeader, const DirectX::ScratchImage& scratchImage)
    {
        STextureMapHeader newTextureMapHeader;
        DirectX::TexMetadata texMetadata = scratchImage.GetMetadata();

        newTextureMapHeader.nNumSlices = texMetadata.depth;
        newTextureMapHeader.nTotalSize = sizeof(ZTextureMap::STextureMapHeader) - 4 + scratchImage.GetPixelsSize();
        newTextureMapHeader.nFlags = oldTextureMapHeader.nFlags;
        newTextureMapHeader.nWidth = static_cast<unsigned short>(texMetadata.width);
        newTextureMapHeader.nHeight = static_cast<unsigned short>(texMetadata.height);
        newTextureMapHeader.nFormat = ZTextureMap::GetRenderFormat(texMetadata.format);
        newTextureMapHeader.nNumMipLevels = static_cast<unsigned char>(texMetadata.mipLevels);
        newTextureMapHeader.nDefaultMipLevel = oldTextureMapHeader.nDefaultMipLevel;
        newTextureMapHeader.nInterpretAs = oldTextureMapHeader.nInterpretAs;
        newTextureMapHeader.nDimensions = oldTextureMapHeader.nDimensions;
        newTextureMapHeader.nMipInterpolation = oldTextureMapHeader.nMipInterpolation;
        newTextureMapHeader.nIADataSize = 0;
        newTextureMapHeader.nIADataOffset = 0;

        return newTextureMapHeader;
    }

    HRESULT ZTextureMap::WriteToMemory(const STextureMapHeader& oldTextureMapHeader, const DirectX::ScratchImage& scratchImage, void** data, unsigned int& dataSize)
    {
        unsigned int capacity = sizeof(STextureMapHeader) + scratchImage.GetPixelsSize();
        HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(capacity);

        return Write(oldTextureMapHeader, scratchImage, binaryWriter);
    }

    HRESULT ZTextureMap::WriteToFile(const STextureMapHeader& oldTextureMapHeader, const DirectX::ScratchImage& scratchImage, const std::string& texPath)
    {
        HM5ResourceTool::IO::ZBinaryWriter binaryWriter = HM5ResourceTool::IO::ZBinaryWriter(texPath);

        return Write(oldTextureMapHeader, scratchImage, binaryWriter);
    }

    HRESULT ZTextureMap::Write(const STextureMapHeader& oldTextureMapHeader, const DirectX::ScratchImage& scratchImage, HM5ResourceTool::IO::ZBinaryWriter binaryWriter)
    {
        STextureMapHeader textureMapHeader = GenerateTextureMapHeader(oldTextureMapHeader, scratchImage);
        DirectX::TexMetadata texMetadata = scratchImage.GetMetadata();
        const DirectX::Image* images = scratchImage.GetImages();
        HRESULT hresult;

        binaryWriter.WriteBytes(&textureMapHeader, sizeof(STextureMapHeader));

        switch (texMetadata.dimension)
        {
        case DirectX::DDS_DIMENSION_TEXTURE1D:
        case DirectX::DDS_DIMENSION_TEXTURE2D:
        {
            size_t index = 0;

            for (size_t item = 0; item < texMetadata.arraySize; ++item)
            {
                for (size_t level = 0; level < texMetadata.mipLevels; ++level, ++index)
                {
                    if (index >= scratchImage.GetImageCount())
                    {
                        return E_FAIL;
                    }

                    if (!images[index].pixels)
                    {
                        return E_POINTER;
                    }

                    assert(images[index].rowPitch > 0);
                    assert(images[index].slicePitch > 0);

                    size_t ddsRowPitch, ddsSlicePitch;

                    hresult = DirectX::ComputePitch(texMetadata.format, images[index].width, images[index].height, ddsRowPitch, ddsSlicePitch, DirectX::CP_FLAGS_NONE);

                    if (FAILED(hresult))
                    {
                        return hresult;
                    }

                    if ((images[index].slicePitch == ddsSlicePitch) && (ddsSlicePitch <= UINT32_MAX))
                    {
                        binaryWriter.WriteBytes(images[index].pixels, ddsSlicePitch);
                    }
                    else
                    {
                        size_t rowPitch = images[index].rowPitch;

                        if (rowPitch < ddsRowPitch)
                        {
                            // DDS uses 1-byte alignment, so if this is happening then the input pitch isn't actually a full line of data
                            return E_FAIL;
                        }

                        if (ddsRowPitch > UINT32_MAX)
                        {
                            return HRESULT_E_ARITHMETIC_OVERFLOW;
                        }

                        uint8_t* __restrict sPtr = images[index].pixels;
                        size_t lines = DirectX::ComputeScanlines(texMetadata.format, images[index].height);

                        for (size_t j = 0; j < lines; ++j)
                        {
                            binaryWriter.WriteBytes(sPtr, ddsRowPitch);

                            sPtr += rowPitch;
                        }
                    }
                }
            }

            break;
        }
        case DirectX::DDS_DIMENSION_TEXTURE3D:
        {
            if (texMetadata.arraySize != 1)
            {
                return E_FAIL;
            }

            size_t depth = texMetadata.depth;
            size_t index = 0;

            for (size_t level = 0; level < texMetadata.mipLevels; ++level)
            {
                for (size_t slice = 0; slice < depth; ++slice, ++index)
                {
                    if (index >= scratchImage.GetImageCount())
                    {
                        return E_FAIL;
                    }

                    if (!images[index].pixels)
                    {
                        return E_POINTER;
                    }

                    assert(images[index].rowPitch > 0);
                    assert(images[index].slicePitch > 0);

                    size_t ddsRowPitch, ddsSlicePitch;

                    hresult = DirectX::ComputePitch(texMetadata.format, images[index].width, images[index].height, ddsRowPitch, ddsSlicePitch, DirectX::CP_FLAGS_NONE);

                    if (FAILED(hresult))
                    {
                        return hresult;
                    }

                    if ((images[index].slicePitch == ddsSlicePitch) && (ddsSlicePitch <= UINT32_MAX))
                    {
                        binaryWriter.WriteBytes(images[index].pixels, ddsSlicePitch);
                    }
                    else
                    {
                        size_t rowPitch = images[index].rowPitch;

                        if (rowPitch < ddsRowPitch)
                        {
                            // DDS uses 1-byte alignment, so if this is happening then the input pitch isn't actually a full line of data
                            return E_FAIL;
                        }

                        if (ddsRowPitch > UINT32_MAX)
                        {
                            return HRESULT_E_ARITHMETIC_OVERFLOW;
                        }

                        uint8_t* __restrict sPtr = images[index].pixels;
                        size_t lines = DirectX::ComputeScanlines(texMetadata.format, images[index].height);

                        for (size_t j = 0; j < lines; ++j)
                        {
                            binaryWriter.WriteBytes(sPtr, ddsRowPitch);

                            sPtr += rowPitch;
                        }
                    }
                }

                if (depth > 1)
                {
                    depth >>= 1;
                }
            }

            break;
        }
        default:
            return E_FAIL;
        }

        return S_OK;
    }
}
