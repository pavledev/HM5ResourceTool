#include "pch.h"
#include "ZResourceLibrary.hpp"
#include "ZUtility.hpp"

namespace HM5ResourceTool
{
	ZResourceLibrary::ZResourceLibrary(const std::string& headerLibraryPath, const std::string& gamePath)
	{
		this->headerLibraryPath = headerLibraryPath;
		this->gamePath = gamePath;
		this->name = "";
		this->resources = new std::vector<ZResource*>();
		this->resourceRuntimeResourceIDs = new std::unordered_map<unsigned long long, unsigned int>();
	}

	ZResourceLibrary::~ZResourceLibrary()
	{
		delete resources;
		delete resourceRuntimeResourceIDs;
	}

	bool ZResourceLibrary::ExtractResourceLibrary(IO::ZBinaryReader& headerBinaryReader, IOI::SHeaderLibraryChunk& headerLibraryChunk, const std::vector<unsigned int>& resouceOffsetsInResourceLib, const std::string exportFolderPath)
	{
		try
		{
			std::string exportFolderPath2;

			if (resourceID.contains("?"))
			{
				size_t startIndex = resourceID.find('?') + 2;
				size_t count = resourceID.find('.', resourceID.find('?')) - resourceID.find('?') - 2;
				
				name = resourceID.substr(startIndex, count);
				exportFolderPath2 = std::format("{}/{}_{}", exportFolderPath, name, std::filesystem::path(filePath).stem().string());
			}
			else
			{
				exportFolderPath2 = std::format("{}/{}", exportFolderPath, std::filesystem::path(filePath).stem().string());
			}

			std::filesystem::create_directories(exportFolderPath2);

			unsigned int resourcePosition = 24;
			unsigned int resourceCount = static_cast<unsigned int>(resouceOffsetsInResourceLib.size());

			IO::ZBinaryReader resourceBinaryReader = IO::ZBinaryReader(filePath);
			IOI::SResourceHeaderHeader* resourceHeaderHeader;
			std::string fileFormat;
			void* resourceData = nullptr;

			for (unsigned int i = 0; i < resourceCount; i++)
			{
				ZResource* resource = new ZResource();

				resource->SetHeaderLibraryPath(headerLibraryPath);
				resource->SetResourceLibraryPath(filePath);

				headerBinaryReader.GetInputStream()->Seek(resouceOffsetsInResourceLib[i], IO::IInputStream::ESeekOrigin::Begin);

				ParseResourceHeader(headerBinaryReader, *resource);

				headerBinaryReader.GetInputStream()->Seek(resouceOffsetsInResourceLib[i], IO::IInputStream::ESeekOrigin::Begin);

				resource->SetHeaderData(headerBinaryReader.GetInputStream()->ReadBytes(resource->GetHeaderDataSize()));

				resourceHeaderHeader = resource->GetResourceHeaderHeader();
				fileFormat = resource->GetFileFormat();

				if (fileFormat == "FSBM" || fileFormat == "FSBS")
				{
					resourceHeaderHeader->m_nDataSize += 24;
				}

				resourceBinaryReader.GetInputStream()->Seek(resourcePosition, IO::IInputStream::ESeekOrigin::Begin);

				resourceData = resourceBinaryReader.ReadBytes(resourceHeaderHeader->m_nDataSize);

				std::string outputFilePath = std::format("{}/{}.{}", exportFolderPath2, i, fileFormat);

				resourcePosition += resourceHeaderHeader->m_nDataSize;

				IO::ZBinaryWriter binaryWriter = IO::ZBinaryWriter(outputFilePath);

				binaryWriter.WriteBytes(resourceData, resourceHeaderHeader->m_nDataSize);

				unsigned int ridMappingIndex = headerLibraryChunk.ridMappingIndices[i];
				unsigned long long hash = headerLibraryChunk.ridMappingIDs[ridMappingIndex];
				IOI::ZRuntimeResourceID runtimeResourceID = IOI::ZRuntimeResourceID::Create(hash);
				std::string resourceID = IOI::ZRuntimeResourceID::QueryResourceID(runtimeResourceID);

				resource->SetRuntimeResourceID(runtimeResourceID);
				resource->SetResourceID(resourceID);
				resource->SetResourceReferenceFlags(IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_NO_FLAGS);
				resource->SetOffsetInHeaderLibrary(resouceOffsetsInResourceLib[i]);
				resource->SetOffsetInResourceLibrary(resourcePosition);
				resource->SetResourceDataSize(resourceHeaderHeader->m_nDataSize);

				resources->push_back(resource);
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

	bool ZResourceLibrary::ParseResourceLibrary(IO::ZBinaryReader& headerBinaryReader, IOI::SHeaderLibraryChunk& headerLibraryChunk,
		const std::vector<unsigned int>& resouceOffsetsInResourceLib, bool readData)
	{
		try
		{
			std::string resourceName = "";

			if (resourceID.contains("?"))
			{
				size_t startIndex = resourceID.find('?') + 2;
				size_t count = resourceID.find('.', resourceID.find('?')) - resourceID.find('?') - 2;

				resourceName = resourceID.substr(startIndex, count);
			}

			unsigned int resourcePosition = 24;
			unsigned int numberOfResourceComponents = static_cast<unsigned int>(resouceOffsetsInResourceLib.size());

			IO::ZBinaryReader* resourceBinaryReader = nullptr;
			IOI::SResourceHeaderHeader* resourceHeaderHeader;
			std::string fileFormat;
			void* resourceData = nullptr;

			if (readData)
			{
				resourceBinaryReader = new IO::ZBinaryReader(filePath);
			}

			for (unsigned int i = 0; i < numberOfResourceComponents; i++)
			{
				ZResource* resource = new ZResource();

				resource->SetHeaderLibraryPath(headerLibraryPath);
				resource->SetResourceLibraryPath(filePath);

				headerBinaryReader.GetInputStream()->Seek(resouceOffsetsInResourceLib[i], IO::IInputStream::ESeekOrigin::Begin);

				ParseResourceHeader(headerBinaryReader, *resource);

				headerBinaryReader.GetInputStream()->Seek(resouceOffsetsInResourceLib[i], IO::IInputStream::ESeekOrigin::Begin);

				resource->SetHeaderData(headerBinaryReader.GetInputStream()->ReadBytes(resource->GetHeaderDataSize()));

				resourceHeaderHeader = resource->GetResourceHeaderHeader();
				fileFormat = resource->GetFileFormat();

				if (fileFormat == "FSBM" || fileFormat == "FSBS")
				{
					resourceHeaderHeader->m_nDataSize += 24;
				}

				if (readData)
				{
					resourceBinaryReader->GetInputStream()->Seek(resourcePosition, IO::IInputStream::ESeekOrigin::Begin);

					resourceData = resourceBinaryReader->ReadBytes(resourceHeaderHeader->m_nDataSize);
				}

				std::string newFileName;

				if (resourceID.contains("?"))
				{
					newFileName = std::format("{}_{}", resourceName, i);
				}
				else
				{
					newFileName = std::format("{}", i);
				}

				unsigned int ridMappingIndex = headerLibraryChunk.ridMappingIndices[i];
				unsigned long long hash = headerLibraryChunk.ridMappingIDs[ridMappingIndex];
				IOI::ZRuntimeResourceID runtimeResourceID = IOI::ZRuntimeResourceID::Create(hash);
				std::string resourceID = IOI::ZRuntimeResourceID::QueryResourceID(runtimeResourceID);

				resource->SetName(GetResourceName(resourceID));
				resource->SetRuntimeResourceID(runtimeResourceID);
				resource->SetResourceID(resourceID);
				resource->SetResourceReferenceFlags(IOI::EResourceReferenceFlags::RESOURCE_REFERENCE_NO_FLAGS);
				resource->SetOffsetInHeaderLibrary(resouceOffsetsInResourceLib[i]);
				resource->SetOffsetInResourceLibrary(resourcePosition);

				if (readData)
				{
					resource->SetResourceData(resourceData);
				}

				resource->SetResourceDataSize(resourceHeaderHeader->m_nDataSize);

				resources->push_back(resource);
				resourceRuntimeResourceIDs->insert(std::make_pair(hash, resourceRuntimeResourceIDs->size()));

				resourcePosition += resourceHeaderHeader->m_nDataSize;
			}

			delete resourceBinaryReader;
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();

			return false;
		}

		return true;
	}

	void ZResourceLibrary::LoadResourceData(const unsigned int resourceIndex)
	{
		try
		{
			IO::ZBinaryReader resourceBinaryReader = IO::ZBinaryReader(filePath);

			resourceBinaryReader.GetInputStream()->Seek(resources->operator[](resourceIndex)->GetOffsetInResourceLibrary(), IO::IInputStream::ESeekOrigin::Begin);

			void* resourceData = resourceBinaryReader.ReadBytes(resources->operator[](resourceIndex)->GetResourceDataSize());

			resources->operator[](resourceIndex)->SetResourceData(resourceData);
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();
		}
	}

	std::string ZResourceLibrary::GetResourceID() const
	{
		return resourceID;
	}

	std::string ZResourceLibrary::GetFilePath() const
	{
		return filePath;
	}

	std::string ZResourceLibrary::GetName() const
	{
		return name;
	}

	const IOI::ZRuntimeResourceID* ZResourceLibrary::GetRuntimeResourceID() const
	{
		return &runtimeResourceID;
	}

	const IOI::SResourceHeaderHeader* ZResourceLibrary::GetResourceHeaderHeader() const
	{
		return &resourceHeaderHeader;
	}

	const std::vector<ZResource*>* ZResourceLibrary::GetResources() const
	{
		return resources;
	}

	IOI::ZRuntimeResourceID* ZResourceLibrary::GetRuntimeResourceID()
	{
		return &runtimeResourceID;
	}

	IOI::SResourceHeaderHeader* ZResourceLibrary::GetResourceHeaderHeader()
	{
		return &resourceHeaderHeader;
	}

	std::vector<ZResource*>* ZResourceLibrary::GetResources()
	{
		return resources;
	}

	void ZResourceLibrary::SetResourceID(std::string& resourceID)
	{
		this->resourceID = resourceID;
	}

	void ZResourceLibrary::SetFilePath(std::string& filePath)
	{
		this->filePath = filePath;
	}

	void ZResourceLibrary::SetName(std::string& name)
	{
		this->name = name;
	}

	void ZResourceLibrary::SetResourceHeaderHeader(IOI::SResourceHeaderHeader& resourceHeaderHeader)
	{
		this->resourceHeaderHeader = resourceHeaderHeader;
	}

	void ZResourceLibrary::SetRuntimeResourceID(IOI::ZRuntimeResourceID& runtimeResourceID)
	{
		this->runtimeResourceID = runtimeResourceID;
	}

	void ZResourceLibrary::SetResources(std::vector<ZResource*>* resources)
	{
		this->resources = resources;
	}

	void ZResourceLibrary::ParseResourceHeader(IO::ZBinaryReader& resourceBinaryReader, ZResource& resource)
	{
		IOI::SResourceHeaderHeader* resourceHeaderHeader = static_cast<IOI::SResourceHeaderHeader*>(resourceBinaryReader.ReadBytes(sizeof(IOI::SResourceHeaderHeader)));

		resource.SetResourceHeaderHeader(*resourceHeaderHeader);
		resource.SetHeaderDataSize(sizeof(IOI::SResourceHeaderHeader) + resourceHeaderHeader->m_nReferencesChunkSize);
	}

	std::string ZResourceLibrary::ConvertTypeToString(unsigned int type)
	{
		std::stringstream stream;

		stream << std::hex << type;

		std::string hex(stream.str());

		unsigned int length = static_cast<unsigned int>(hex.length());
		std::string result;

		for (unsigned int i = 0; i < length; i += 2)
		{
			std::string byte = hex.substr(i, 2);
			char chr = static_cast<char>(static_cast<int>(strtol(byte.c_str(), nullptr, 16)));

			result.push_back(chr);
		}

		return result;
	}

	unsigned int ZResourceLibrary::GetResourceFilePosition(IOI::ZRuntimeResourceID& ridResource)
	{
		int index = ridResource.GetIndexInLibrary();

		return resources->operator[](index)->GetOffsetInResourceLibrary();
	}

	void ZResourceLibrary::SerializeToJSON(const IOI::SHeaderLibraryChunk& headerLibraryChunk, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		std::string resourceID = headerLibraryChunk.sLibraryID.ToCString();

		writer.StartObject();

		writer.String("sLibraryID");
		writer.String(resourceID.c_str());

		writer.String("filePath");
		writer.String(filePath.c_str());

		writer.String("hash");
		writer.String(ZUtility::ConvertValueToHexString(runtimeResourceID.GetID()).c_str());

		writer.String("nOffset");
		writer.String(ZUtility::ConvertValueToHexString(headerLibraryChunk.nOffset).c_str());

		writer.String("nFlags");
		writer.Int(headerLibraryChunk.nFlags);

		writer.String("localizationCategory");
		writer.Uint(headerLibraryChunk.localizationCategory);

		writer.String("sLanguages");
		writer.StartArray();

		for (unsigned int j = 0; j < headerLibraryChunk.sLanguages.Size(); ++j)
		{
			writer.String(headerLibraryChunk.sLanguages[j].ToCString());
		}

		writer.EndArray();

		writer.String("states");
		writer.StartArray();

		for (unsigned int j = 0; j < headerLibraryChunk.states.Size(); ++j)
		{
			writer.Int(headerLibraryChunk.states[j]);
		}

		writer.EndArray();

		writer.String("resources");
		writer.StartArray();

		for (unsigned int j = 0; j < resources->size(); ++j)
		{
			ZResource* resource = resources->operator[](j);

			resource->SerializeToJSON(writer);
		}

		writer.EndArray();

		writer.EndObject();
	}

	int ZResourceLibrary::GetIndexOfResource(const unsigned long long hash)
	{
		auto it = resourceRuntimeResourceIDs->find(hash);

		if (it != resourceRuntimeResourceIDs->end())
		{
			return it->second;
		}

		return -1;
	}

	void ZResourceLibrary::GenerateResourceLibrary()
	{
		IO::ZBinaryWriter binaryWriter = IO::ZBinaryWriter(filePath);

		binaryWriter.WriteBytes(&resourceHeaderHeader, sizeof(IOI::SResourceHeaderHeader));

		for (unsigned int i = 0; i < resources->size(); ++i)
		{
			binaryWriter.WriteBytes(resources->operator[](i)->GetResourceData(), resources->operator[](i)->GetResourceDataSize());
		}
	}

	std::string ZResourceLibrary::GetResourceName(const std::string& resourceID)
	{
		std::string name = "";

		if (resourceID.length() > 0)
		{
			if (resourceID.contains("("))
			{
				size_t index = resourceID.substr(0, resourceID.find("(")).find_last_of('/') + 1;

				name = resourceID.substr(index, resourceID.find("(") - 1 - index);
			}
			else
			{
				if (resourceID.contains("?"))
				{
					name = resourceID.substr(resourceID.find("?") + 2, resourceID.find("]") - resourceID.find("?") - 2);
				}
				else
				{
					name = resourceID.substr(resourceID.find_last_of("/") + 1, resourceID.find("]") - resourceID.find_last_of("/") - 1);
				}
			}
		}

		return name;
	}
}
