#include "pch.h"
#include <format>
#include <iostream>
#include "ZTexture.hpp"
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDS.h"
#include "Glacier2/ZTextureMap.hpp"

namespace HM5ResourceTool
{
	bool ZTexture::ConvertTextureMapToDDS(const std::string& textPath, const std::string& outputFilePath)
	{
		try
		{
			std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(textPath);

			ConvertTextureMapToDDS(textureMap, outputFilePath2);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToDDS(const void* data, const unsigned int dataSize, void** ddsData, unsigned int& ddsSize)
	{
		try
		{
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(data, dataSize);
			DirectX::ScratchImage* scratchImage = textureMap.GetScratchImage();
			DirectX::Blob blob;

			HRESULT hresult = DirectX::SaveToDDSMemory(*scratchImage->GetImages(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, blob);

			ddsSize = blob.GetBufferSize();
			*ddsData = operator new(ddsSize);

			memcpy_s(*ddsData, ddsSize, blob.GetBufferPointer(), ddsSize);

			if (!SUCCEEDED(hresult))
			{
				std::string message = std::format("Failed to convert texture to DDS. {}", GetErrorMessage(hresult));

				std::cout << message;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToDDS(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		try
		{
			std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(data, dataSize);
			std::wstring filePath2 = std::wstring(outputFilePath2.begin(), outputFilePath2.end());

			ConvertTextureMapToDDS(textureMap, filePath2);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToDDS(IOI::ZTextureMap& textureMap, const std::wstring& outputFilePath)
	{
		DirectX::ScratchImage* scratchImage = textureMap.GetScratchImage();

		HRESULT hresult = DirectX::SaveToDDSFile(*scratchImage->GetImages(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, outputFilePath.c_str());

		if (SUCCEEDED(hresult))
		{
			std::cout << "Texture Map file successfully converted to DDS.";
		}
		else
		{
			std::string message = std::format("Failed to convert texture to DDS. {}", GetErrorMessage(hresult));

			std::cout << message;

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToTGA(const std::string& textPath, const std::string& outputFilePath)
	{
		try
		{
			std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(textPath);

			ConvertTextureMapToTGA(textureMap, outputFilePath2);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToTGA(const void* data, const unsigned int dataSize, void** tgaData, unsigned int& tgaSize)
	{
		try
		{
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(data, dataSize);
			const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

			DirectX::ScratchImage* scratchImage = textureMap.GetScratchImage();
			const DirectX::Image* image = scratchImage->GetImage(0, 0, 0);
			DirectX::ScratchImage scratchImage2;
			const DirectX::Image* image2;
			HRESULT hresult;

			if (textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_CUBE ||
				textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_VOLUME)
			{
				std::cout << "Cube map and 3D textures aren't supported. Use DDS to export them.";

				return false;
			}

			IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMapHeader->nFormat);
			DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);
			DirectX::Blob blob;

			if (DirectX::IsCompressed(dxgiFormat))
			{
				hresult = DirectX::Decompress(*image, DXGI_FORMAT_UNKNOWN, scratchImage2);

				if (FAILED(hresult))
				{
					std::cout << "Failed to decompress texture.";

					return false;
				}

				image2 = scratchImage2.GetImage(0, 0, 0);
			}
			else if (DirectX::IsPlanar(dxgiFormat))
			{
				hresult = DirectX::ConvertToSinglePlane(*image, scratchImage2);

				if (FAILED(hresult))
				{
					std::cout << "Failed to convert texture to single plane.";

					return false;
				}

				image2 = scratchImage2.GetImage(0, 0, 0);
			}
			else
			{
				image2 = scratchImage->GetImage(0, 0, 0);
			}

			hresult = DirectX::SaveToTGAMemory(*image2, blob, textureMap.GetTexMetadata());

			tgaSize = blob.GetBufferSize();
			*tgaData = operator new(tgaSize);

			memcpy_s(*tgaData, tgaSize, blob.GetBufferPointer(), tgaSize);

			if (!SUCCEEDED(hresult))
			{
				std::string message = std::format("Failed to convert texture to TGA. {}", GetErrorMessage(hresult));

				std::cout << message;

				return false;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToTGA(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		try
		{
			std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(data, dataSize);

			ConvertTextureMapToTGA(textureMap, outputFilePath2);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToTGA(IOI::ZTextureMap& textureMap, const std::wstring& outputFilePath)
	{
		const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

		DirectX::ScratchImage* scratchImage = textureMap.GetScratchImage();
		const DirectX::Image* image = scratchImage->GetImage(0, 0, 0);
		DirectX::ScratchImage scratchImage2;
		const DirectX::Image* image2;
		HRESULT hresult;

		if (textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_CUBE ||
			textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_VOLUME)
		{
			std::cout << "Cube map and 3D textures aren't supported. Use DDS to export them.";

			return false;
		}

		IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMapHeader->nFormat);
		DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);

		if (DirectX::IsCompressed(dxgiFormat))
		{
			hresult = DirectX::Decompress(*image, DXGI_FORMAT_UNKNOWN, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to decompress texture.";

				return false;
			}

			image2 = scratchImage2.GetImage(0, 0, 0);
		}
		else if (DirectX::IsPlanar(dxgiFormat))
		{
			hresult = DirectX::ConvertToSinglePlane(*image, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to convert texture to single plane.";

				return false;
			}

			image2 = scratchImage2.GetImage(0, 0, 0);
		}
		else
		{
			image2 = scratchImage->GetImage(0, 0, 0);
		}

		hresult = DirectX::SaveToTGAFile(*image2, outputFilePath.c_str(), textureMap.GetTexMetadata());

		if (SUCCEEDED(hresult))
		{
			std::cout << "Texture Map file successfully converted to TGA.";
		}
		else
		{
			std::string message = std::format("Failed to convert texture to TGA. {}", GetErrorMessage(hresult));

			std::cout << message;

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToBMP(const std::string& textPath, const std::string& outputFilePath)
	{
		try
		{
			std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(textPath);

			ConvertTextureMapToBMP(textureMap, outputFilePath2);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToBMP(const void* data, const unsigned int dataSize, void** bmpData, unsigned int& bmpSize)
	{
		try
		{
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(data, dataSize);
			const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

			DirectX::ScratchImage* scratchImage = textureMap.GetScratchImage();
			const DirectX::Image* image = scratchImage->GetImage(0, 0, 0);
			DirectX::ScratchImage scratchImage2;
			const DirectX::Image* image2;
			HRESULT hresult;

			if (textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_CUBE ||
				textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_VOLUME)
			{
				std::cout << "Cube map and 3D textures aren't supported. Use DDS to export them.";

				return false;
			}

			IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMapHeader->nFormat);
			DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);
			DirectX::Blob blob;

			if (DirectX::IsCompressed(dxgiFormat))
			{
				hresult = DirectX::Decompress(*image, DXGI_FORMAT_UNKNOWN, scratchImage2);

				if (FAILED(hresult))
				{
					std::cout << "Failed to decompress texture.";

					return false;
				}

				image2 = scratchImage2.GetImage(0, 0, 0);
			}
			else if (DirectX::IsPlanar(dxgiFormat))
			{
				hresult = DirectX::ConvertToSinglePlane(*image, scratchImage2);

				if (FAILED(hresult))
				{
					std::cout << "Failed to convert texture to single plane.";

					return false;
				}

				image2 = scratchImage2.GetImage(0, 0, 0);
			}
			else
			{
				image2 = scratchImage->GetImage(0, 0, 0);
			}

			if (dxgiFormat == DXGI_FORMAT_R8G8B8A8_UNORM)
			{
				hresult = DirectX::SaveToWICMemory(*image2, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatBmp, blob, &GUID_WICPixelFormat32bppBGR);
			}
			else
			{
				hresult = DirectX::SaveToWICMemory(*image2, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatBmp, blob);
			}

			bmpSize = blob.GetBufferSize();
			*bmpData = operator new(bmpSize);

			memcpy_s(*bmpData, bmpSize, blob.GetBufferPointer(), bmpSize);

			if (!SUCCEEDED(hresult))
			{
				std::string message = std::format("Failed to convert texture to BMP. {}", GetErrorMessage(hresult));

				std::cout << message;

				return false;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToBMP(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		try
		{
			std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(data, dataSize);

			ConvertTextureMapToBMP(textureMap, outputFilePath2);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToBMP(IOI::ZTextureMap& textureMap, const std::wstring& outputFilePath)
	{
		const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

		DirectX::ScratchImage* scratchImage = textureMap.GetScratchImage();
		const DirectX::Image* image = scratchImage->GetImage(0, 0, 0);
		DirectX::ScratchImage scratchImage2;
		const DirectX::Image* image2;
		HRESULT hresult;

		if (textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_CUBE ||
			textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_VOLUME)
		{
			std::cout << "Cube map and 3D textures aren't supported. Use DDS to export them.";

			return false;
		}

		IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMapHeader->nFormat);
		DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);

		if (DirectX::IsCompressed(dxgiFormat))
		{
			hresult = DirectX::Decompress(*image, DXGI_FORMAT_UNKNOWN, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to decompress texture.";

				return false;
			}

			image2 = scratchImage2.GetImage(0, 0, 0);
		}
		else if (DirectX::IsPlanar(dxgiFormat))
		{
			hresult = DirectX::ConvertToSinglePlane(*image, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to convert texture to single plane.";

				return false;
			}

			image2 = scratchImage2.GetImage(0, 0, 0);
		}
		else
		{
			image2 = scratchImage->GetImage(0, 0, 0);
		}

		hresult = DirectX::SaveToWICFile(*image2, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatBmp, outputFilePath.c_str());
		//DirectX::SaveToWICFile(scratchImage->GetImages(), scratchImage->GetImageCount(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatBmp, bmpPath.c_str());

		if (SUCCEEDED(hresult))
		{
			std::cout << "Texture Map file successfully converted to BMP.";
		}
		else
		{
			std::string message = std::format("Failed to convert texture to BMP. {}", GetErrorMessage(hresult));

			std::cout << message;

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToPNG(const std::string& textPath, const std::string& outputFilePath)
	{
		try
		{
			std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(textPath);

			ConvertTextureMapToPNG(textureMap, outputFilePath2);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToPNG(const void* data, const unsigned int dataSize, void** pngData, unsigned int& pngSize)
	{
		try
		{
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(data, dataSize);
			const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

			DirectX::ScratchImage* scratchImage = textureMap.GetScratchImage();
			const DirectX::Image* image = scratchImage->GetImage(0, 0, 0);
			DirectX::ScratchImage scratchImage2;
			const DirectX::Image* image2;
			HRESULT hresult;

			if (textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_CUBE ||
				textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_VOLUME)
			{
				std::cout << "Cube map and 3D textures aren't supported. Use DDS to export them.";

				return false;
			}

			IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMapHeader->nFormat);
			DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);
			DirectX::Blob blob;

			if (DirectX::IsCompressed(dxgiFormat))
			{
				hresult = DirectX::Decompress(*image, DXGI_FORMAT_UNKNOWN, scratchImage2);

				if (FAILED(hresult))
				{
					std::cout << "Failed to decompress texture.";

					return false;
				}

				image2 = scratchImage2.GetImage(0, 0, 0);
			}
			else if (DirectX::IsPlanar(dxgiFormat))
			{
				hresult = DirectX::ConvertToSinglePlane(*image, scratchImage2);

				if (FAILED(hresult))
				{
					std::cout << "Failed to convert texture to single plane.";

					return false;
				}

				image2 = scratchImage2.GetImage(0, 0, 0);
			}
			else
			{
				image2 = scratchImage->GetImage(0, 0, 0);
			}

			if (dxgiFormat == DXGI_FORMAT_R8G8B8A8_UNORM)
			{
				hresult = DirectX::SaveToWICMemory(*image2, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatPng, blob, &GUID_WICPixelFormat32bppBGR);
			}
			else
			{
				hresult = DirectX::SaveToWICMemory(*image2, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatPng, blob);
			}

			pngSize = blob.GetBufferSize();
			*pngData = operator new(pngSize);

			memcpy_s(*pngData, pngSize, blob.GetBufferPointer(), pngSize);

			if (!SUCCEEDED(hresult))
			{
				std::string message = std::format("Failed to convert texture to PNG. {}", GetErrorMessage(hresult));

				std::cout << message;

				return false;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToPNG(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		try
		{
			std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(data, dataSize);

			ConvertTextureMapToPNG(textureMap, outputFilePath2);
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTextureMapToPNG(IOI::ZTextureMap& textureMap, const std::wstring& outputFilePath)
	{
		const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

		DirectX::ScratchImage* scratchImage = textureMap.GetScratchImage();
		const DirectX::Image* image = scratchImage->GetImage(0, 0, 0);
		DirectX::ScratchImage scratchImage2;
		const DirectX::Image* image2;
		HRESULT hresult = 0;

		if (textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_CUBE ||
			textureMapHeader->nDimensions == IOI::ZTextureMap::DIMENSIONS_VOLUME)
		{
			std::cout << "Cube map and 3D textures aren't supported. Use DDS to export them.";

			return false;
		}

		IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMapHeader->nFormat);
		DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);

		if (DirectX::IsCompressed(dxgiFormat))
		{
			hresult = DirectX::Decompress(*image, DXGI_FORMAT_UNKNOWN, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to decompress texture.";

				return false;
			}

			image2 = scratchImage2.GetImage(0, 0, 0);
		}
		else if (DirectX::IsPlanar(dxgiFormat))
		{
			hresult = DirectX::ConvertToSinglePlane(*image, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to convert texture to single plane.";

				return false;
			}

			image2 = scratchImage2.GetImage(0, 0, 0);
		}
		else
		{
			image2 = scratchImage->GetImage(0, 0, 0);
		}

		DirectX::SaveToWICFile(*image2, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatPng, outputFilePath.c_str());
		//DirectX::SaveToWICFile(scratchImage->GetImages(), scratchImage->GetImageCount(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatPng, pngPath.c_str());

		if (!SUCCEEDED(hresult))
		{
			std::string message = std::format("Failed to convert texture to PNG. {}", GetErrorMessage(hresult));

			std::cout << message;

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertDDSToTextureMap(const std::string& ddsPath, void** data, unsigned int& dataSize)
	{
		try
		{
			std::string textPath = std::format("{}.TEXT", ddsPath.substr(0, ddsPath.find_last_of('.')));
			std::wstring ddsPath2 = std::wstring(ddsPath.begin(), ddsPath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(textPath);
			const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

			DirectX::TexMetadata texMetadata;
			DirectX::ScratchImage scratchImage;

			HRESULT hresult = DirectX::LoadFromDDSFile(ddsPath2.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &texMetadata, scratchImage);

			if (FAILED(hresult))
			{
				std::cout << "Failed to load data from DDS file.";

				return false;
			}

			hresult = IOI::ZTextureMap::WriteToMemory(*textureMapHeader, scratchImage, data, dataSize);

			if (!SUCCEEDED(hresult))
			{
				std::string message = std::format("Failed to convert DDS to Texture Map. {}", GetErrorMessage(hresult));

				std::cout << message;

				return false;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertTGAToTextureMap(const std::string& tgaPath, void** data, unsigned int& dataSize)
	{
		try
		{
			std::string textPath = std::format("{}.TEXT", tgaPath.substr(0, tgaPath.find_last_of('.')));
			std::wstring tgaPath2 = std::wstring(tgaPath.begin(), tgaPath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(textPath);
			const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

			DirectX::TexMetadata texMetadata;
			DirectX::ScratchImage scratchImage;
			DirectX::ScratchImage scratchImage2;

			HRESULT hresult = DirectX::LoadFromTGAFile(tgaPath2.c_str(), &texMetadata, scratchImage);

			if (FAILED(hresult))
			{
				std::cout << "Failed to load data from TGA file.";

				return false;
			}

			const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);

			hresult = DirectX::GenerateMipMaps(*image, DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, texMetadata.mipLevels, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to generate mip maps.";

				return false;
			}

			IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMap.GetHeader()->nFormat);
			DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);
			DirectX::ScratchImage scratchImage3;

			if (DirectX::IsCompressed(dxgiFormat))
			{
				hresult = DirectX::Compress(scratchImage2.GetImages(), scratchImage2.GetImageCount(), scratchImage2.GetMetadata(), dxgiFormat, DirectX::TEX_COMPRESS_FLAGS::TEX_COMPRESS_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, scratchImage3);

				if (FAILED(hresult))
				{
					std::cout << "Failed to compress texture.";

					return false;
				}
			}

			hresult = IOI::ZTextureMap::WriteToMemory(*textureMapHeader, scratchImage3, data, dataSize);

			if (!SUCCEEDED(hresult))
			{
				std::string message = std::format("Failed to convert TGA to Texture Map. {}", GetErrorMessage(hresult));

				std::cout << message;

				return false;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertBMPToTextureMap(const std::string& bmpPath, void** data, unsigned int& dataSize)
	{
		try
		{
			std::string textPath = std::format("{}.TEXT", bmpPath.substr(0, bmpPath.find_last_of('.')));
			std::wstring bmpPath2 = std::wstring(bmpPath.begin(), bmpPath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(textPath);
			const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

			DirectX::TexMetadata texMetadata;
			DirectX::ScratchImage scratchImage;
			DirectX::ScratchImage scratchImage2;

			HRESULT hresult = DirectX::LoadFromWICFile(bmpPath2.c_str(), DirectX::WIC_FLAGS_NONE, &texMetadata, scratchImage);

			if (FAILED(hresult))
			{
				std::cout << "Failed to load data from BMP file.";

				return false;
			}

			const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);

			hresult = DirectX::GenerateMipMaps(*image, DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, texMetadata.mipLevels, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to generate mip maps.";

				return false;
			}

			IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMap.GetHeader()->nFormat);
			DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);
			DirectX::ScratchImage scratchImage3;

			if (DirectX::IsCompressed(dxgiFormat))
			{
				hresult = DirectX::Compress(scratchImage2.GetImages(), scratchImage2.GetImageCount(), scratchImage2.GetMetadata(), dxgiFormat, DirectX::TEX_COMPRESS_FLAGS::TEX_COMPRESS_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, scratchImage3);

				if (FAILED(hresult))
				{
					std::cout << "Failed to compress texture.";

					return false;
				}
			}

			hresult = IOI::ZTextureMap::WriteToMemory(*textureMapHeader, scratchImage3, data, dataSize);

			if (!SUCCEEDED(hresult))
			{
				std::string message = std::format("Failed to convert BMP to Texture Map. {}", GetErrorMessage(hresult));

				std::cout << message;

				return false;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZTexture::ConvertPNGToTextureMap(const std::string& pngPath, void** data, unsigned int& dataSize)
	{
		try
		{
			std::string textPath = std::format("{}.TEXT", pngPath.substr(0, pngPath.find_last_of('.')));
			std::wstring pngPath2 = std::wstring(pngPath.begin(), pngPath.end());
			IOI::ZTextureMap textureMap = IOI::ZTextureMap(textPath);
			const IOI::ZTextureMap::STextureMapHeader* textureMapHeader = textureMap.GetHeader();

			DirectX::TexMetadata texMetadata;
			DirectX::ScratchImage scratchImage;
			DirectX::ScratchImage scratchImage2;

			HRESULT hresult = DirectX::LoadFromWICFile(pngPath2.c_str(), DirectX::WIC_FLAGS_NONE, &texMetadata, scratchImage);

			if (FAILED(hresult))
			{
				std::cout << "Failed to load data from PNG file.";

				return false;
			}

			const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);

			hresult = DirectX::GenerateMipMaps(*image, DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, texMetadata.mipLevels, scratchImage2);

			if (FAILED(hresult))
			{
				std::cout << "Failed to generate mip maps.";

				return false;
			}

			IOI::ERenderFormat renderFormat = static_cast<IOI::ERenderFormat>(textureMap.GetHeader()->nFormat);
			DXGI_FORMAT dxgiFormat = textureMap.GetDXGIFormat(renderFormat);
			DirectX::ScratchImage scratchImage3;

			if (DirectX::IsCompressed(dxgiFormat))
			{
				hresult = DirectX::Compress(scratchImage2.GetImages(), scratchImage2.GetImageCount(), scratchImage2.GetMetadata(), dxgiFormat, DirectX::TEX_COMPRESS_FLAGS::TEX_COMPRESS_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, scratchImage3);

				if (FAILED(hresult))
				{
					std::cout << "Failed to compress texture.";

					return false;
				}
			}

			hresult = IOI::ZTextureMap::WriteToMemory(*textureMapHeader, scratchImage3, data, dataSize);

			if (SUCCEEDED(hresult))
			{
				std::cout << "PNG file successfully converted to Texture Map.";
			}
			else
			{
				std::string message = std::format("Failed to convert PNG to Texture Map. {}", GetErrorMessage(hresult));

				return false;
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	std::string ZTexture::GetErrorMessage(HRESULT hresult)
	{
		std::string message;

		switch (hresult)
		{
		case HRESULT_E_ARITHMETIC_OVERFLOW:
			message = "Arithmetic result exceeded 32 bits.";

			break;
		case HRESULT_E_NOT_SUPPORTED:
			message = "The request is not supported.";

			break;
		case HRESULT_E_HANDLE_EOF:
			message = "Reached the end of the file.";

			break;
		case HRESULT_E_INVALID_DATA:
			message = "The data is invalid.";

			break;
		case HRESULT_E_FILE_TOO_LARGE:
			message = "The file size exceeds the limit allowed and cannot be saved.";

			break;
		case HRESULT_E_CANNOT_MAKE:
			message = "The directory or file cannot be created.";

			break;
		case E_NOT_SUFFICIENT_BUFFER:
			message = "The data area passed to a system call is too small.";

			break;
		default:
			break;
		}

		return message;
	}
}
