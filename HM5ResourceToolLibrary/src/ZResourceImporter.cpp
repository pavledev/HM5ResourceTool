#include "pch.h"
#include <filesystem>
#include "ZResourceImporter.hpp"
#include "ZHeaderLibrary.hpp"
#include "ZEntityTemplate.hpp"
#include "ZBehaviorTree.hpp"
#include "ZTexture.hpp"
#include "ZScaleform.hpp"
#include "ZAudio.hpp"
#include "ZVideo.hpp"

namespace HM5ResourceTool
{
	ZResourceImporter::ZResourceImporter(const std::string& metaDataFilePath, const std::string& gamePath)
	{
		IO::ZJSONDeserializer jsonDeserializer = IO::ZJSONDeserializer(metaDataFilePath);

		resource = jsonDeserializer.Deserialize<ZResource>();
        this->gamePath = gamePath;
	}

    ZResourceImporter::~ZResourceImporter()
    {
        delete resource;
    }

    bool ZResourceImporter::ImportResource(const std::string& inputFilePath, bool backupResourceLibrary)
    {
        bool result = false;
        std::string fileFormat = resource->GetFileFormat();
        void* data = nullptr;
        unsigned int dataSize = 0;

        std::reverse(fileFormat.begin(), fileFormat.end());

        if (fileFormat == "TEMP")
        {
            result = ImportTEMP(inputFilePath, &data, dataSize);
        }
        else if (fileFormat == "TBLU")
        {
            result = ImportTBLU(inputFilePath, &data, dataSize);
        }
        else if (fileFormat == "CPPT")
        {
            result = ImportCPPT(inputFilePath, &data, dataSize);
        }
        else if (fileFormat == "CBLU")
        {
            result = ImportCBLU(inputFilePath, &data, dataSize);
        }
        else if (fileFormat == "AIBB")
        {
            result = ImportAIBB(inputFilePath, &data, dataSize);
        }
        else if (fileFormat == "TEXT")
        {
            result = ImportTEXT(inputFilePath, &data, dataSize);
        }
        else if (fileFormat == "FSBF")
        {

        }
        else if (fileFormat == "FSBM")
        {

        }
        else if (fileFormat == "FSBS")
        {

        }
        else if (fileFormat == "WAVB")
        {

        }
        else if (fileFormat == "MUCB")
        {

        }
        else if (fileFormat == "SWFF")
        {

        }
        else if (fileFormat == "GFXF")
        {

        }
        else if (fileFormat == "TELI")
        {

        }
        else if (fileFormat == "PRIM")
        {

        }
        else if (fileFormat == "MATI")
        {

        }
        else if (fileFormat == "MATT")
        {

        }
        else if (fileFormat == "MATB")
        {

        }

        //IO::ZBinaryReader resourceBinaryReader = IO::ZBinaryReader(inputFilePath);

        //unsigned int newSize = resourceBinaryReader.GetInputStream()->GetSize();

        std::string resourceLibraryPath = resource->GetResourceLibraryPath();
        std::string resourceLibraryPath2 = resourceLibraryPath.insert(resourceLibraryPath.length() - 14, "2");
        std::string resourceLibraryPath3 = resourceLibraryPath.insert(resourceLibraryPath.length() - 14, "_Original");
        IO::ZBinaryReader resourceLibraryBinaryReader = IO::ZBinaryReader(resourceLibraryPath);
        //unsigned int capacity = resourceLibraryBinaryReader.GetInputStream()->GetSize() - resource.GetResourceDataSize() + newSize;
        //IO::ZBinaryWriter resourceLibraryBinaryWriter = IO::ZBinaryWriter(capacity);
        IO::ZBinaryWriter resourceLibraryBinaryWriter = IO::ZBinaryWriter(resourceLibraryPath2);

        resourceLibraryBinaryWriter.WriteBytes(resourceLibraryBinaryReader.ReadBytes(resource->GetOffsetInResourceLibrary()), resource->GetOffsetInResourceLibrary());
        resourceLibraryBinaryWriter.WriteBytes(data, dataSize);

        unsigned int oldSize = resource->GetOffsetInResourceLibrary() + resource->GetResourceDataSize();

        resourceLibraryBinaryReader.GetInputStream()->Seek(oldSize, IO::IInputStream::ESeekOrigin::Begin);
        resourceLibraryBinaryWriter.WriteBytes(resourceLibraryBinaryReader.ReadBytes(oldSize), oldSize);

        if (backupResourceLibrary)
        {
            std::filesystem::copy(resourceLibraryPath, resourceLibraryPath3);
        }

        std::filesystem::remove(resourceLibraryPath);
        std::filesystem::rename(resourceLibraryPath2, resourceLibraryPath);

        /*ZHeaderLibrary headerLibrary = ZHeaderLibrary(resource.GetHeaderLibraryPath());

        headerLibrary.ParseHeaderLibrary(gamePath, nullptr, false);

        std::vector<ZResourceLibrary*>* resourceLibraries = headerLibrary.GetResourceLibraries();

        for (unsigned int i = 0; i < resourceLibraries->size(); ++i)
        {

        }*/

        std::fstream fstream = std::fstream(resource->GetHeaderLibraryPath());

        fstream.seekp(resource->GetOffsetInHeaderLibrary());
        fstream.write(reinterpret_cast<const char*>(&dataSize), sizeof(unsigned int));

        fstream.close();

        return result;
    }

	bool ZResourceImporter::ImportTEMP(const std::string& inputFilePath, void** data, unsigned int& dataSize)
	{
		return ZEntityTemplate::ConvertJSONToTemplateEntity(inputFilePath, resource->GetReferences(), data, dataSize);
	}

    bool ZResourceImporter::ImportTBLU(const std::string& inputFilePath, void** data, unsigned int& dataSize)
    {
        return ZEntityTemplate::ConvertJSONToTemplateEntityBlueprint(inputFilePath, data, dataSize);
    }

    bool ZResourceImporter::ImportCPPT(const std::string& inputFilePath, void** data, unsigned int& dataSize)
    {
        return ZEntityTemplate::ConvertJSONToCppEntity(inputFilePath, data, dataSize);
    }

    bool ZResourceImporter::ImportCBLU(const std::string& inputFilePath, void** data, unsigned int& dataSize)
    {
        return ZEntityTemplate::ConvertJSONToCppEntityBlueprint(inputFilePath, data, dataSize);
    }

    bool ZResourceImporter::ImportAIBB(const std::string& inputFilePath, void** data, unsigned int& dataSize)
    {
        return ZBehaviorTree::ConvertJSONToBehaviorTreeEntityBlueprint(inputFilePath, data, dataSize);
    }

    bool ZResourceImporter::ImportTEXT(const std::string& inputFilePath, void** data, unsigned int& dataSize)
    {
        bool result = false;
        std::string extension = inputFilePath.substr(inputFilePath.length() - 3, 3);

        if (extension == "dds")
        {
            result = ZTexture::ConvertDDSToTextureMap(inputFilePath, data, dataSize);
        }
        else if (extension == "tga")
        {
            result = ZTexture::ConvertTGAToTextureMap(inputFilePath, data, dataSize);
        }
        else if (extension == "bmp")
        {
            result = ZTexture::ConvertBMPToTextureMap(inputFilePath, data, dataSize);
        }
        else if (extension == "png")
        {
            result = ZTexture::ConvertPNGToTextureMap(inputFilePath, data, dataSize);
        }

        return result;
    }
}
