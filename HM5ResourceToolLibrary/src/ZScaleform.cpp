#include "pch.h"
#include "ZScaleform.hpp"
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTexP.h"
#include "DirectXTex/DDS.h"

namespace HM5ResourceTool
{
	bool ZScaleform::Convert(const std::string& swffPath, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(swffPath);

			Convert(binaryReader, outputFilePath);
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

	bool ZScaleform::Convert(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);

			Convert(binaryReader, outputFilePath);
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

	void ZScaleform::Convert(IO::ZBinaryReader& binaryReader, const std::string& outputFilePath)
	{
		binaryReader.ReadByte();

		unsigned int fileOffset = binaryReader.ReadUInt32();

		binaryReader.GetInputStream()->Seek(fileOffset + 1, IO::IInputStream::ESeekOrigin::Begin);

		unsigned int currentPosition = binaryReader.GetInputStream()->GetPosition();

		binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

		unsigned int size = binaryReader.GetInputStream()->GetPosition() - currentPosition;

		binaryReader.GetInputStream()->Seek(fileOffset + 1, IO::IInputStream::ESeekOrigin::Begin);

		IO::ZBinaryWriter binaryWriter(outputFilePath);

		binaryWriter.WriteBytes(binaryReader.GetInputStream()->GetCurrentPointer(), size);
	}

	bool ZScaleform::ConvertSWFFToPNG(const void* data, const unsigned int dataSize, const std::string& outputFilePath, bool isSwf)
	{
		try
		{
			IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);

			binaryReader.ReadByte();

			unsigned int fileOffset = binaryReader.ReadUInt32();

			binaryReader.GetInputStream()->Seek(fileOffset + 1, IO::IInputStream::ESeekOrigin::Begin);

			unsigned int currentPosition = binaryReader.GetInputStream()->GetPosition();

			binaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

			unsigned int size = binaryReader.GetInputStream()->GetPosition() - currentPosition;

			binaryReader.GetInputStream()->Seek(fileOffset + 1, IO::IInputStream::ESeekOrigin::Begin);

			if (isSwf)
			{
				Swf swf;
				RenderBuffer buf;

				SWFRender::ReadSWF(&swf, binaryReader.GetInputStream()->GetCurrentPointer(), size);
				SWFRender::Init(&buf, 0, 0, (swf.movieSize.xmax - swf.movieSize.xmin) / 20,
					(swf.movieSize.ymax - swf.movieSize.ymin) / 20, 2, 1);
				SWFRender::RenderSWF(&buf, &swf);

				RGBA* img = SWFRender::Render(&buf);

				PNG::Write(outputFilePath.c_str(), reinterpret_cast<unsigned char*>(img), buf.width, buf.height);
				SWFRender::Delete(&buf);
			}
			else
			{
				DirectX::TexMetadata texMetadata;
				DirectX::ScratchImage scratchImage;
				DirectX::ScratchImage scratchImage2;
				std::wstring outputFilePath2 = std::wstring(outputFilePath.begin(), outputFilePath.end());

				DirectX::LoadFromDDSMemory(data, dataSize, DirectX::DDS_FLAGS_NONE, &texMetadata, scratchImage);

				const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);
				const DirectX::Image* image2;
				HRESULT hresult = 0;

				if (DirectX::IsCompressed(texMetadata.format))
				{
					hresult = DirectX::Decompress(*image, DXGI_FORMAT_UNKNOWN, scratchImage2);

					if (FAILED(hresult))
					{
						std::cout << "Failed to decompress DDS.";

						return false;
					}

					image2 = scratchImage2.GetImage(0, 0, 0);
				}
				else if (DirectX::IsPlanar(texMetadata.format))
				{
					hresult = DirectX::ConvertToSinglePlane(*image, scratchImage2);

					if (FAILED(hresult))
					{
						std::cout << "Failed to convert DDS to single plane.";

						return false;
					}

					image2 = scratchImage2.GetImage(0, 0, 0);
				}
				else
				{
					image2 = scratchImage.GetImage(0, 0, 0);
				}

				DirectX::SaveToWICFile(*image2, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, GUID_ContainerFormatPng, outputFilePath2.c_str());

				if (!SUCCEEDED(hresult))
				{
					std::string message = std::format("Failed to convert DDS to PNG. {}", GetErrorMessage(hresult));

					std::cout << message;

					return false;
				}
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	bool ZScaleform::ConvertGFXFToGFX(const std::string& gfxfPath, const std::string& outputFilePath)
	{
		try
		{
			std::string gfxPath = std::format("{}.gfx", gfxfPath.substr(0, gfxfPath.find_last_of('.')));
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SGfxResource* gfxResource = static_cast<IOI::SGfxResource*>(binaryDeserializer.Deserialize(gfxfPath));

			ConvertGFXFToGFX(gfxResource, gfxPath, binaryDeserializer.GetAlignment());
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

	bool ZScaleform::ConvertGFXFToGFX(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		try
		{
			IOI::ZBinaryDeserializer binaryDeserializer;
			IOI::SGfxResource* gfxResource = static_cast<IOI::SGfxResource*>(binaryDeserializer.Deserialize(data, dataSize));

			ConvertGFXFToGFX(gfxResource, outputFilePath, binaryDeserializer.GetAlignment());
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

	void ZScaleform::ConvertGFXFToGFX(IOI::SGfxResource* gfxResource, const std::string& outputFilePath, unsigned char alignment)
	{
		IO::ZBinaryWriter binaryWriter = IO::ZBinaryWriter(outputFilePath);

		binaryWriter.WriteBytes(gfxResource->gfxData, gfxResource->gfxDataLength);

		std::string path = outputFilePath.substr(0, outputFilePath.find_last_of('\\'));

		for (unsigned int i = 0; i < gfxResource->filenames.Size(); i++)
		{
			unsigned int size = gfxResource->fileContents[i].Size();
			IO::ZBinaryWriter binaryWriter = IO::ZBinaryWriter(std::format("{}\\{}", path, gfxResource->filenames[i].ToCString()));

			binaryWriter.WriteBytes(gfxResource->fileContents[i].GetStart(), size);
		}

		operator delete(gfxResource, std::align_val_t(alignment));
	}

	bool ZScaleform::ConvertDDSToSWFF(const std::string& ddsPath)
	{
		try
		{
			std::string swffPath = std::format("{}.SWFF", ddsPath.substr(0, ddsPath.find_last_of('.')));
			IO::ZBinaryReader ddsBinaryReader = IO::ZBinaryReader(ddsPath);
			IO::ZBinaryReader swffBinaryReader = IO::ZBinaryReader(swffPath);

			ConvertToSWFF(ddsBinaryReader, swffBinaryReader, ddsPath);
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

	bool ZScaleform::ConvertPNGToSWFF(const std::string& pngPath)
	{
		try
		{
			std::string swffPath = std::format("{}.SWFF", pngPath.substr(0, pngPath.find_last_of('.')));
			IO::ZBinaryReader pngBinaryReader = IO::ZBinaryReader(pngPath);
			IO::ZBinaryReader swffBinaryReader = IO::ZBinaryReader(swffPath);

			ConvertToSWFF(pngBinaryReader, swffBinaryReader, pngPath);
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

	bool ZScaleform::ConvertSWFToSWFF(const std::string& swfPath)
	{
		try
		{
			std::string swffPath = std::format("{}.SWFF", swfPath.substr(0, swfPath.find_last_of('.')));
			IO::ZBinaryReader swfBinaryReader = IO::ZBinaryReader(swfPath);
			IO::ZBinaryReader swffBinaryReader = IO::ZBinaryReader(swffPath);

			ConvertToSWFF(swfBinaryReader, swffBinaryReader, swfPath);
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

	bool ZScaleform::ConvertToSWFF(IO::ZBinaryReader& inputBinaryReader, IO::ZBinaryReader& swffBinaryReader, const std::string& inputFilePath)
	{
		try
		{
			std::string swffPath = std::format("{}.SWFF", inputFilePath.substr(0, inputFilePath.find_last_of('.')));
			unsigned int unknownValue = swffBinaryReader.ReadByte();
			unsigned int fileOffset = swffBinaryReader.ReadUInt32();
			std::string filePath = swffBinaryReader.ReadString();

			inputBinaryReader.GetInputStream()->Seek(0, IO::IInputStream::ESeekOrigin::End);

			IO::ZBinaryWriter binaryWriter(swffPath);

			binaryWriter.WriteByte(unknownValue);
			binaryWriter.WriteUInt32(fileOffset);
			binaryWriter.WriteString(filePath);
			binaryWriter.WriteBytes(inputBinaryReader.GetInputStream()->GetBuffer(), inputBinaryReader.GetInputStream()->GetPosition());
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

	std::string ZScaleform::GetErrorMessage(HRESULT hresult)
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
