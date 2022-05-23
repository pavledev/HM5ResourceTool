#include "pch.h"
#include "ZResourceExporter.hpp"
#include "ZEntityTemplate.hpp"
#include "ZBehaviorTree.hpp"
#include "ZTexture.hpp"
#include "ZScaleform.hpp"
#include "ZAudio.hpp"
#include "ZVideo.hpp"
#include "ZPackageList.hpp"
#include "ZTextList.hpp"
#include "ZHeaderLibrary.hpp"
#include "ZModel.hpp"
#include "ZMaterial.hpp"

namespace HM5ResourceTool
{
	bool ZResourceExporter::WriteJSONToFile(const std::string& jsonOutput, const std::string& filePath)
	{
		std::ofstream ofstream;

		ofstream.open(filePath);

		if (ofstream.is_open())
		{
			ofstream << jsonOutput;

			ofstream.close();

			return true;
		}

		return false;
	}

	bool ZResourceExporter::WriteToTextFile(const std::string& output, const std::string& filePath)
	{
		std::ofstream ofstream;

		ofstream.open(filePath);

		if (ofstream.is_open())
		{
			ofstream << output;

			ofstream.close();

			return true;
		}

		return false;
	}

	bool ZResourceExporter::ExportTEMP(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;
		const std::string metaJSONFilePath = std::format("{}.meta.json", inputFilePath.substr(0, inputFilePath.find_last_of('.')));
		IO::ZJSONDeserializer jsonDeserializer = IO::ZJSONDeserializer(metaJSONFilePath);
		ZResource* resource = jsonDeserializer.Deserialize<ZResource>();

		ZEntityTemplate::ConvertTemplateEntityToJSON(resource->GetReferences(), inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		delete resource;

		return false;
	}

	bool ZResourceExporter::ExportTBLU(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZEntityTemplate::ConvertTemplateEntityBlueprintToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportCPPT(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;
		const std::string metaJSONFilePath = std::format("{}.meta.json", inputFilePath.substr(0, inputFilePath.find_last_of('.')));
		IO::ZJSONDeserializer jsonDeserializer = IO::ZJSONDeserializer(metaJSONFilePath);
		ZResource* resource = jsonDeserializer.Deserialize<ZResource>();

		bool result = ZEntityTemplate::ConvertCppEntityToJSON(resource->GetReferences(), inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportCBLU(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZEntityTemplate::ConvertCppEntityBlueprintToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportAIBB(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZBehaviorTree::ConvertBehaviorTreeEntityBlueprintToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportTEXT(const std::string& inputFilePath, const std::string& outputFilePath, unsigned int option)
	{
		bool result = false;

		switch (option)
		{
		case 0:
			result = ZTexture::ConvertTextureMapToDDS(inputFilePath, outputFilePath);
			break;
		case 1:
			result = ZTexture::ConvertTextureMapToTGA(inputFilePath, outputFilePath);
			break;
		case 2:
			result = ZTexture::ConvertTextureMapToBMP(inputFilePath, outputFilePath);
			break;
		case 3:
			result = ZTexture::ConvertTextureMapToPNG(inputFilePath, outputFilePath);
			break;
		}

		return result;
	}

	bool ZResourceExporter::ExportSWFF(const std::string& inputFilePath, std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(inputFilePath);

		binaryReader.ReadByte();

		unsigned int fileOffset = binaryReader.ReadInt32();

		binaryReader.GetInputStream()->Seek(fileOffset + 1, IO::IInputStream::ESeekOrigin::Begin);

		char* chars = binaryReader.ReadChars(3);
		std::string magic = std::string(chars, 3);
		bool result = false;

		delete[] chars;

		if (magic == "DDS")
		{
			outputFilePath += ".dds";

			result = ZScaleform::Convert(inputFilePath, outputFilePath);
		}
		else if (magic == "PNG")
		{
			outputFilePath += ".png";

			result = ZScaleform::Convert(inputFilePath, outputFilePath);
		}
		else if (magic == "CWF")
		{
			outputFilePath += ".swf";

			result = ZScaleform::Convert(inputFilePath, outputFilePath);
		}

		return result;
	}

	bool ZResourceExporter::ExportGFXF(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		return ZScaleform::ConvertGFXFToGFX(inputFilePath, outputFilePath);
	}

	bool ZResourceExporter::ExportFSBF(const std::string& inputFilePath, std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(inputFilePath);
		bool result;

		if (ZAudio::IsOGGFormat(binaryReader, 16))
		{
			outputFilePath += ".ogg";

			result = ZAudio::ConvertFSBFToOGG(inputFilePath, outputFilePath);
		}
		else
		{
			outputFilePath += ".wav";

			result = ZAudio::ConvertFSBFToWAV(inputFilePath, outputFilePath);
		}

		return result;
	}

	bool ZResourceExporter::ExportFSBM(const std::string& inputFilePath, std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(inputFilePath);
		bool result;

		if (ZAudio::IsOGGFormat(binaryReader, 24))
		{
			outputFilePath += ".ogg";

			result = ZAudio::ConvertFSBFToOGG(inputFilePath, outputFilePath);
		}
		else
		{
			outputFilePath += ".wav";

			result = ZAudio::ConvertFSBFToWAV(inputFilePath, outputFilePath);
		}

		return result;
	}

	bool ZResourceExporter::ExportFSBS(const std::string& inputFilePath, std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(inputFilePath);
		bool result;

		if (ZAudio::IsOGGFormat(binaryReader, ZAudio::FindFSB5DataOffsetInFSBS(binaryReader)))
		{
			outputFilePath += ".ogg";

			result = ZAudio::ConvertFSBFToOGG(inputFilePath, outputFilePath);
		}
		else
		{
			outputFilePath += ".wav";

			result = ZAudio::ConvertFSBFToWAV(inputFilePath, outputFilePath);
		}

		return result;
	}

	bool ZResourceExporter::ExportWAVB(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZAudio::ConvertWAVBToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportMUCB(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZAudio::ConvertMUCBToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportTELI(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string output;

		bool result = ZTextList::ConvertTextListToText(inputFilePath, output);

		WriteToTextFile(output, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportPRIM(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		return ZModel::ConvertRenderPrimitiveToGLTF(inputFilePath, outputFilePath);
	}

	bool ZResourceExporter::ExportMATI(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZMaterial::ConvertRenderMaterialInstanceToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportMATT(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZMaterial::ConvertRenderMaterialEntityToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportMATB(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZMaterial::ConvertRenderMaterialEntityBlueprintToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportTEMP(ZResource* resource, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZEntityTemplate::ConvertTemplateEntityToJSON(resource->GetReferences(), resource->GetResourceData(), resource->GetResourceDataSize(), jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportTBLU(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZEntityTemplate::ConvertTemplateEntityBlueprintToJSON(data, dataSize, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportCPPT(ZResource* resource, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZEntityTemplate::ConvertCppEntityToJSON(resource->GetReferences(), resource->GetResourceData(), resource->GetResourceDataSize(), jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportCBLU(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZEntityTemplate::ConvertCppEntityBlueprintToJSON(data, dataSize, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportAIBB(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZBehaviorTree::ConvertBehaviorTreeEntityBlueprintToJSON(data, dataSize, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportTEXT(const void* data, const unsigned int dataSize, const std::string& outputFilePath, unsigned int option)
	{
		switch (option)
		{
		case 0:
			ZTexture::ConvertTextureMapToDDS(data, dataSize, outputFilePath);
			break;
		case 1:
			ZTexture::ConvertTextureMapToTGA(data, dataSize, outputFilePath);
			break;
		case 2:
			ZTexture::ConvertTextureMapToBMP(data, dataSize, outputFilePath);
			break;
		case 3:
			ZTexture::ConvertTextureMapToPNG(data, dataSize, outputFilePath);
			break;
		}

		return false;
	}

	bool ZResourceExporter::ExportSWFF(const void* data, const unsigned int dataSize, std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);

		binaryReader.ReadByte();

		unsigned int fileOffset = binaryReader.ReadInt32();

		binaryReader.GetInputStream()->Seek(fileOffset + 1, IO::IInputStream::ESeekOrigin::Begin);

		char* chars = binaryReader.ReadChars(3);
		std::string magic = std::string(chars, 3);
		bool result = false;

		delete[] chars;

		if (magic == "DDS")
		{
			outputFilePath += ".dds";

			result = ZScaleform::ConvertSWFFToPNG(data, dataSize, outputFilePath);
		}
		else if (magic == "PNG")
		{
			outputFilePath += ".png";

			result = ZScaleform::Convert(data, dataSize, outputFilePath);
		}
		else if (magic == "CWF")
		{
			outputFilePath += ".swf";

			result = ZScaleform::ConvertSWFFToPNG(data, dataSize, outputFilePath);
		}

		return result;
	}

	bool ZResourceExporter::ExportGFXF(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		return ZScaleform::ConvertGFXFToGFX(data, dataSize, outputFilePath);
	}

	bool ZResourceExporter::ExportFSBF(const void* data, const unsigned int dataSize, std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
		bool result;

		if (ZAudio::IsOGGFormat(binaryReader, 16))
		{
			outputFilePath += ".ogg";

			result = ZAudio::ConvertFSBFToOGG(data, dataSize, outputFilePath);
		}
		else
		{
			outputFilePath += ".wav";

			result = ZAudio::ConvertFSBFToWAV(data, dataSize, outputFilePath);
		}

		return result;
	}

	bool ZResourceExporter::ExportFSBM(const void* data, const unsigned int dataSize, std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
		bool result;

		if (ZAudio::IsOGGFormat(binaryReader, 16))
		{
			outputFilePath += ".ogg";

			result = ZAudio::ConvertFSBMToOGG(data, dataSize, outputFilePath);
		}
		else
		{
			outputFilePath += ".wav";

			result = ZAudio::ConvertFSBMToWAV(data, dataSize, outputFilePath);
		}

		return result;
	}

	bool ZResourceExporter::ExportFSBS(const void* data, const unsigned int dataSize, std::string& outputFilePath)
	{
		IO::ZBinaryReader binaryReader = IO::ZBinaryReader(data, dataSize);
		bool result;

		if (ZAudio::IsOGGFormat(binaryReader, 16))
		{
			outputFilePath += ".ogg";

			result = ZAudio::ConvertFSBSToOGG(data, dataSize, outputFilePath);
		}
		else
		{
			outputFilePath += ".wav";

			result = ZAudio::ConvertFSBSToWAV(data, dataSize, outputFilePath);
		}

		return result;
	}

	bool ZResourceExporter::ExportWAVB(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZAudio::ConvertWAVBToJSON(data, dataSize, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportMUCB(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZAudio::ConvertMUCBToJSON(data, dataSize, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportTELI(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string output;

		bool result = ZTextList::ConvertTextListToText(data, dataSize, output);

		WriteToTextFile(output, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportPRIM(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		return ZModel::ConvertRenderPrimitiveToGLTF(data, dataSize, outputFilePath);
	}

	bool ZResourceExporter::ExportMATI(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZMaterial::ConvertRenderMaterialInstanceToJSON(data, dataSize, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportMATT(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZMaterial::ConvertRenderMaterialEntityToJSON(data, dataSize, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportMATB(const void* data, const unsigned int dataSize, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZMaterial::ConvertRenderMaterialEntityBlueprintToJSON(data, dataSize, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}

	bool ZResourceExporter::ExportPCBinkVid(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		return ZVideo::ConvertPCBinkVidToBINK(inputFilePath, outputFilePath);
	}

	bool ZResourceExporter::ExportPackageList(const std::string& inputFilePath, const std::string& outputFilePath)
	{
		std::string jsonOutput;

		bool result = ZPackageList::ConvertPackgeListToJSON(inputFilePath, jsonOutput);

		WriteJSONToFile(jsonOutput, outputFilePath);

		return result;
	}
}
