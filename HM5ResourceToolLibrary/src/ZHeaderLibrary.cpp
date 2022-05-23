#include "pch.h"
#include "ZHeaderLibrary.hpp"
#include "Glacier2/ZBinaryDeserializer.hpp"
#include "ZUtility.hpp"
#include "ZResourceExporter.hpp"
#include "Glacier2/Entity/STemplateEntity.hpp"
#include "Glacier2/Entity/STemplateEntityBlueprint.hpp"

namespace HM5ResourceTool
{
	ZHeaderLibrary::ZHeaderLibrary(const std::string& headerLibraryPath)
	{
		this->headerLibraryPath = headerLibraryPath;
		this->headerBinaryReader = new HM5ResourceTool::IO::ZBinaryReader(headerLibraryPath);
		this->resourceHeaderHeader = static_cast<IOI::SResourceHeaderHeader*>(headerBinaryReader->ReadBytes(sizeof(IOI::SResourceHeaderHeader)));
		this->references = new std::vector<ZResource>();
		this->resourceLibraries = new std::vector<ZResourceLibrary*>();

		ParseReferencesChunk();
		DeserializeHeaderLibrary();

		headerBinaryReader->GetInputStream()->Seek(0, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);
	}

	ZHeaderLibrary::~ZHeaderLibrary()
	{
		delete headerBinaryReader;
		delete resourceHeaderHeader;
		delete references;
		delete resourceLibraries;
		operator delete(headerLibrary, std::align_val_t(alignment));
	}

	bool ZHeaderLibrary::ParseHeaderLibrary(const std::string& gamePath, ZHeaderLibrary* globalHeaderLibrary, bool readData)
	{
		try
		{
			headerBinaryReader->ReadInt32();
			headerBinaryReader->GetInputStream()->Seek(headerBinaryReader->ReadInt32() + 64, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);

			unsigned int numberOfResourceIDs = headerBinaryReader->ReadUInt32();
			std::vector<unsigned int> resourceIDOffsets(numberOfResourceIDs);
			std::vector<unsigned int> resourceOffsetsInHeaderlib(numberOfResourceIDs);

			headerBinaryReader->ReadInt32();

			for (unsigned int i = 0; i < numberOfResourceIDs; ++i)
			{
				resourceIDOffsets[i] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

				headerBinaryReader->GetInputStream()->Seek(36, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);

				resourceOffsetsInHeaderlib[i] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

				headerBinaryReader->GetInputStream()->Seek(36, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);
			}

			for (unsigned int i = 0; i < numberOfResourceIDs; ++i)
			{
				headerBinaryReader->GetInputStream()->Seek(resourceIDOffsets[i] - 4, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

				int resourceIDLength = headerBinaryReader->ReadUInt32();
				std::string resourceID = headerBinaryReader->ReadString(resourceIDLength - 1);
				std::string filePath = IOI::ZLocalResourceFileMediator::CalcResourceFileName(resourceID);

				if (headerLibraryPath.contains("DLC"))
				{
					filePath = std::format("{}{}", headerLibraryPath.substr(0, headerLibraryPath.find_last_of("\\runtime")), filePath.substr(2));
				}
				else
				{
					filePath = std::format("{}{}", gamePath, filePath.substr(2));
				}

				if (!std::filesystem::exists(filePath))
				{
					continue;
				}

				headerBinaryReader->GetInputStream()->Seek(resourceOffsetsInHeaderlib[i] - 4, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

				unsigned int numberOfResourceComponents = headerBinaryReader->ReadUInt32();
				std::vector<unsigned int> resouceOffsetsInResourceLib(numberOfResourceComponents);

				for (unsigned int j = 0; j < numberOfResourceComponents; ++j)
				{
					resouceOffsetsInResourceLib[j] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

					headerBinaryReader->ReadUInt64();
				}

				ZResourceLibrary* resourceLibrary = new ZResourceLibrary(this->headerLibraryPath, gamePath);

				resourceLibrary->SetResourceID(resourceID);
				resourceLibrary->SetFilePath(filePath);

				IOI::ZRuntimeResourceID runtimeResourceID = IOI::ZRuntimeResourceID::QueryRuntimeResourceID(resourceID);

				resourceLibrary->SetRuntimeResourceID(runtimeResourceID);

				headerLibrary->chunks[i].nOffset = resourceIDOffsets[i];

				if (resourceLibrary->ParseResourceLibrary(*headerBinaryReader, headerLibrary->chunks[i], resouceOffsetsInResourceLib, readData))
				{
					resourceLibraries->push_back(resourceLibrary);
				}
				else
				{
					return false;
				}
			}

			if (readData)
			{
				for (unsigned int i = 0; i < resourceLibraries->size(); ++i)
				{
					ZResourceLibrary* resourceLibrary = resourceLibraries->operator[](i);
					std::vector<ZResource*>* resources = resourceLibrary->GetResources();

					for (unsigned int j = 0; j < resources->size(); ++j)
					{
						ZResource* resource2 = resources->operator[](j);

						headerBinaryReader->GetInputStream()->Seek(resource2->GetOffsetInHeaderLibrary() + sizeof(IOI::SResourceHeaderHeader), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

						ParseReferencesChunk(*headerBinaryReader, *resource2, globalHeaderLibrary);
					}
				}
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

		headerBinaryReader->GetInputStream()->Seek(0, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

		return true;
	}

	bool ZHeaderLibrary::ExtractResourceLibrary(const std::string& gamePath, ZHeaderLibrary* globalHeaderLibrary, const std::string& exportFolderPath, const std::string& resourceID)
	{
		try
		{
			headerBinaryReader->ReadInt32();
			headerBinaryReader->GetInputStream()->Seek(headerBinaryReader->ReadInt32() + 64, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);

			unsigned int numberOfResourceIDs = headerBinaryReader->ReadUInt32();
			std::vector<unsigned int> resourceIDOffsets(numberOfResourceIDs);
			std::vector<unsigned int> resourceOffsetsInHeaderlib(numberOfResourceIDs);

			headerBinaryReader->ReadInt32();

			for (unsigned int i = 0; i < numberOfResourceIDs; ++i)
			{
				resourceIDOffsets[i] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

				headerBinaryReader->GetInputStream()->Seek(36, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);

				resourceOffsetsInHeaderlib[i] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

				headerBinaryReader->GetInputStream()->Seek(36, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);
			}

			ZResourceLibrary* resourceLibrary = nullptr;

			for (unsigned int i = 0; i < numberOfResourceIDs; ++i)
			{
				headerBinaryReader->GetInputStream()->Seek(resourceIDOffsets[i] - 4, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

				int resourceIDLength = headerBinaryReader->ReadUInt32();
				std::string resourceID2 = headerBinaryReader->ReadString(resourceIDLength - 1);

				if (resourceID == resourceID2)
				{
					std::string filePath = IOI::ZLocalResourceFileMediator::CalcResourceFileName(resourceID);

					if (headerLibraryPath.contains("DLC"))
					{
						filePath = std::format("{}{}", headerLibraryPath.substr(0, headerLibraryPath.find_last_of("\\runtime")), filePath.substr(2));
					}
					else
					{
						filePath = std::format("{}{}", gamePath, filePath.substr(2));
					}

					if (!std::filesystem::exists(filePath))
					{
						continue;
					}

					headerBinaryReader->GetInputStream()->Seek(resourceOffsetsInHeaderlib[i] - 4, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

					unsigned int numberOfResourceComponents = headerBinaryReader->ReadUInt32();
					std::vector<unsigned int> resouceOffsetsInResourceLib(numberOfResourceComponents);

					for (unsigned int j = 0; j < numberOfResourceComponents; ++j)
					{
						resouceOffsetsInResourceLib[j] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

						headerBinaryReader->ReadUInt64();
					}

					resourceLibrary = new ZResourceLibrary(this->headerLibraryPath, gamePath);

					resourceLibrary->SetResourceID(resourceID2);
					resourceLibrary->SetFilePath(filePath);

					IOI::ZRuntimeResourceID runtimeResourceID = IOI::ZRuntimeResourceID::QueryRuntimeResourceID(resourceID);

					resourceLibrary->SetRuntimeResourceID(runtimeResourceID);

					if (!resourceLibrary->ExtractResourceLibrary(*headerBinaryReader, headerLibrary->chunks[i], resouceOffsetsInResourceLib, exportFolderPath))
					{
						return false;
					}

					break;
				}
			}

			std::vector<ZResource*>* resources = resourceLibrary->GetResources();
			std::string exportFolderPath2;

			if (resourceLibrary->GetName().length() > 0)
			{
				exportFolderPath2 = std::format("{}/{}_{}", exportFolderPath, resourceLibrary->GetName(), std::filesystem::path(resourceLibrary->GetFilePath()).stem().string());
			}
			else
			{
				exportFolderPath2 = std::format("{}/{}", exportFolderPath, std::filesystem::path(resourceLibrary->GetFilePath()).stem().string());
			}

			for (unsigned int i = 0; i < resources->size(); ++i)
			{
				ZResource* resource2 = resources->operator[](i);

				headerBinaryReader->GetInputStream()->Seek(resource2->GetOffsetInHeaderLibrary() + sizeof(IOI::SResourceHeaderHeader), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

				ParseReferencesChunk(*headerBinaryReader, *resource2, globalHeaderLibrary);

				const std::string jsonOutput = resource2->SerializeToJSON();
				const std::string metadataJsonFilePath = std::format("{}/{}.meta.json", exportFolderPath2, i);

				ZResourceExporter::WriteJSONToFile(jsonOutput, metadataJsonFilePath);
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

		headerBinaryReader->GetInputStream()->Seek(0, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

		return true;
	}

	bool ZHeaderLibrary::ExtractResourceLibraries(const std::string& gamePath, ZHeaderLibrary* globalHeaderLibrary, const std::string& exportFolderPath)
	{
		try
		{
			headerBinaryReader->ReadInt32();
			headerBinaryReader->GetInputStream()->Seek(headerBinaryReader->ReadInt32() + 64, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);

			unsigned int numberOfResourceIDs = headerBinaryReader->ReadUInt32();
			std::vector<unsigned int> resourceIDOffsets(numberOfResourceIDs);
			std::vector<unsigned int> resourceOffsetsInHeaderlib(numberOfResourceIDs);

			headerBinaryReader->ReadInt32();

			for (unsigned int i = 0; i < numberOfResourceIDs; ++i)
			{
				resourceIDOffsets[i] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

				headerBinaryReader->GetInputStream()->Seek(36, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);

				resourceOffsetsInHeaderlib[i] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

				headerBinaryReader->GetInputStream()->Seek(36, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);
			}

			for (unsigned int i = 0; i < numberOfResourceIDs; ++i)
			{
				headerBinaryReader->GetInputStream()->Seek(resourceIDOffsets[i] - 4, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

				int resourceIDLength = headerBinaryReader->ReadUInt32();
				std::string resourceID = headerBinaryReader->ReadString(resourceIDLength - 1);
				std::string filePath = IOI::ZLocalResourceFileMediator::CalcResourceFileName(resourceID);

				if (headerLibraryPath.contains("DLC"))
				{
					filePath = std::format("{}{}", headerLibraryPath.substr(0, headerLibraryPath.find_last_of("\\runtime")), filePath.substr(2));
				}
				else
				{
					filePath = std::format("{}{}", gamePath, filePath.substr(2));
				}

				if (!std::filesystem::exists(filePath))
				{
					continue;
				}

				headerBinaryReader->GetInputStream()->Seek(resourceOffsetsInHeaderlib[i] - 4, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

				unsigned int numberOfResourceComponents = headerBinaryReader->ReadUInt32();
				std::vector<unsigned int> resouceOffsetsInResourceLib(numberOfResourceComponents);

				for (unsigned int j = 0; j < numberOfResourceComponents; ++j)
				{
					resouceOffsetsInResourceLib[j] = static_cast<unsigned int>(headerBinaryReader->GetInputStream()->GetPosition()) + headerBinaryReader->ReadInt32();

					headerBinaryReader->ReadUInt64();
				}

				ZResourceLibrary* resourceLibrary = new ZResourceLibrary(this->headerLibraryPath, gamePath);

				resourceLibrary->SetResourceID(resourceID);
				resourceLibrary->SetFilePath(filePath);

				IOI::ZRuntimeResourceID runtimeResourceID = IOI::ZRuntimeResourceID::QueryRuntimeResourceID(resourceID);

				resourceLibrary->SetRuntimeResourceID(runtimeResourceID);

				if (resourceLibrary->ExtractResourceLibrary(*headerBinaryReader, headerLibrary->chunks[i], resouceOffsetsInResourceLib, exportFolderPath))
				{
					resourceLibraries->push_back(resourceLibrary);
				}
				else
				{
					return false;
				}
			}

			for (unsigned int i = 0; i < resourceLibraries->size(); ++i)
			{
				ZResourceLibrary* resourceLibrary = resourceLibraries->operator[](i);
				std::vector<ZResource*>* resources = resourceLibrary->GetResources();

				std::string exportFolderPath2;

				if (resourceLibrary->GetName().length() > 0)
				{
					exportFolderPath2 = std::format("{}/{}_{}", exportFolderPath, resourceLibrary->GetName(), std::filesystem::path(resourceLibrary->GetFilePath()).stem().string());
				}
				else
				{
					exportFolderPath2 = std::format("{}/{}", exportFolderPath, std::filesystem::path(resourceLibrary->GetFilePath()).stem().string());
				}

				for (unsigned int j = 0; j < resources->size(); ++j)
				{
					ZResource* resource2 = resources->operator[](j);

					headerBinaryReader->GetInputStream()->Seek(resource2->GetOffsetInHeaderLibrary() + sizeof(IOI::SResourceHeaderHeader), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

					ParseReferencesChunk(*headerBinaryReader, *resource2, globalHeaderLibrary);

					const std::string jsonOutput = resource2->SerializeToJSON();
					const std::string metadataJsonFilePath = std::format("{}/{}.meta.json", exportFolderPath2, j);

					ZResourceExporter::WriteJSONToFile(jsonOutput, metadataJsonFilePath);
				}
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

		headerBinaryReader->GetInputStream()->Seek(0, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

		return true;
	}

	void ZHeaderLibrary::ParseReferencesChunk()
	{
		const unsigned char* referencesChunk = nullptr;

		if (resourceHeaderHeader->m_nReferencesChunkSize > 0)
		{
			referencesChunk = static_cast<unsigned char*>(headerBinaryReader->GetInputStream()->ReadBytes(resourceHeaderHeader->m_nReferencesChunkSize));
		}

		const IOI::ZResourceHeaderReader resourceHeaderReader = IOI::ZResourceHeaderReader(*resourceHeaderHeader, referencesChunk);
		const unsigned int numberOfReferences = resourceHeaderReader.GetNumResourceIdentifiers();

		for (unsigned int i = 0; i < numberOfReferences; ++i)
		{
			const IOI::ZRuntimeResourceID runtimeResourceID = resourceHeaderReader.GetResourceIdentifier(i);
			const std::string resourceID = IOI::ZRuntimeResourceID::QueryResourceID(runtimeResourceID);
			const IOI::EResourceReferenceFlags flags = resourceHeaderReader.GetResourceFlags(i);
			ZResource reference;

			reference.SetRuntimeResourceID(runtimeResourceID);
			reference.SetResourceID(resourceID);
			reference.SetResourceReferenceFlags(flags);

			references->push_back(reference);
		}

		if (referencesChunk)
		{
			delete[] referencesChunk;
		}
	}

	void ZHeaderLibrary::ParseReferencesChunk(HM5ResourceTool::IO::ZBinaryReader& headerBinaryReader, ZResource& resource, ZHeaderLibrary* globalHeaderLibrary)
	{
		const IOI::SResourceHeaderHeader* resourceHeaderHeader = resource.GetResourceHeaderHeader();
		const unsigned char* referencesChunk = nullptr;

		if (resourceHeaderHeader->m_nReferencesChunkSize > 0)
		{
			referencesChunk = static_cast<unsigned char*>(headerBinaryReader.GetInputStream()->ReadBytes(resourceHeaderHeader->m_nReferencesChunkSize));
		}

		const IOI::ZResourceHeaderReader resourceHeaderReader = IOI::ZResourceHeaderReader(*resourceHeaderHeader, referencesChunk);
		const unsigned int numberOfReferences = resourceHeaderReader.GetNumResourceIdentifiers();

		for (unsigned int i = 0; i < numberOfReferences; ++i)
		{
			const IOI::ZRuntimeResourceID runtimeResourceID = resourceHeaderReader.GetResourceIdentifier(i);
			const IOI::EResourceReferenceFlags flags = resourceHeaderReader.GetResourceFlags(i);
			ZResource* resourceReference = nullptr;
			int libraryIndex = 0;

			if (runtimeResourceID.IsLibraryResource())
			{
				libraryIndex = GetIndexOfResourceLibrary(runtimeResourceID);

				//Some resources reference resources from libraries which don't exist
				if (libraryIndex == -1)
				{
					resourceReference = new ZResource();

					resourceReference->SetRuntimeResourceID(runtimeResourceID);

					resource.AddReference(resourceReference); //References need to be added to keep same indices for deserializer

					continue;
				}

				const int indexInLibrary = runtimeResourceID.GetIndexInLibrary();
				std::vector<ZResource*>* resources = resourceLibraries->operator[](libraryIndex)->GetResources();
				resourceReference = resources->operator[](indexInLibrary);

				resourceReference->SetResourceReferenceFlags(flags);

				resource.AddReference(resourceReference);
			}
			else
			{
				if (globalHeaderLibrary)
				{
					libraryIndex = globalHeaderLibrary->GetIndexOfResourceLibrary(runtimeResourceID, true);

					//Some resources reference resources from libraries which don't exist
					if (libraryIndex == -1)
					{
						resourceReference = new ZResource();

						resourceReference->SetRuntimeResourceID(runtimeResourceID);

						resource.AddReference(resourceReference); //References need to be added to keep same indices for deserializer

						continue;
					}

					std::vector<ZResourceLibrary*>* resourceLibraries = globalHeaderLibrary->GetResourceLibraries();
					std::vector<ZResource*>* resources = resourceLibraries->operator[](libraryIndex)->GetResources();
					int resourceIndex = globalHeaderLibrary->GetIndexOfResource(runtimeResourceID);

					resourceReference = resources->operator[](resourceIndex);
				}
				else
				{
					resourceReference = new ZResource();
				}

				resourceReference->SetRuntimeResourceID(runtimeResourceID);
				resourceReference->SetResourceID(IOI::ZRuntimeResourceID::QueryResourceID(runtimeResourceID));
				resourceReference->SetResourceReferenceFlags(flags);

				resource.AddReference(resourceReference);
			}
		}

		if (referencesChunk)
		{
			delete[] referencesChunk;
		}
	}

	void ZHeaderLibrary::LoadResourceData(const unsigned int libraryIndex, const unsigned int resourceIndex)
	{
		resourceLibraries->operator[](libraryIndex)->LoadResourceData(resourceIndex);
	}

	void ZHeaderLibrary::LoadResourceReferences(const unsigned int libraryIndex, const unsigned int resourceIndex, ZHeaderLibrary* globalHeaderLibrary)
	{
		std::vector<ZResource*>* resources = resourceLibraries->operator[](libraryIndex)->GetResources();
		ZResource* resource = resources->operator[](resourceIndex);

		headerBinaryReader->GetInputStream()->Seek(resource->GetOffsetInHeaderLibrary() + sizeof(IOI::SResourceHeaderHeader), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

		ParseReferencesChunk(*headerBinaryReader, *resource, globalHeaderLibrary);
	}

	std::vector<ZResource>* ZHeaderLibrary::GetReferences()
	{
		return references;
	}

	const std::vector<ZResource>* ZHeaderLibrary::GetReferences() const
	{
		return references;
	}

	std::vector<ZResourceLibrary*>* ZHeaderLibrary::GetResourceLibraries()
	{
		return resourceLibraries;
	}

	void ZHeaderLibrary::DeserializeHeaderLibrary()
	{
		IOI::ZBinaryDeserializer binaryDeserializer;

		headerBinaryReader->GetInputStream()->Seek(4, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

		unsigned int referencesChunkSize = headerBinaryReader->ReadUInt32();

		headerBinaryReader->GetInputStream()->Seek(referencesChunkSize + 24, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

		headerLibrary = static_cast<IOI::SHeaderLibrary*>(binaryDeserializer.Deserialize(*headerBinaryReader));
		alignment = binaryDeserializer.GetAlignment();
	}

	void ZHeaderLibrary::SerializeToJSON(std::string& jsonOutput)
	{
		rapidjson::StringBuffer stringBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

		writer.StartObject();

		writer.String("resourceHeaderHeader");
		resourceHeaderHeader->SerializeToJSON(writer);

		writer.String("references");
		writer.StartArray();

		std::stringstream stream;
		std::string result(stream.str());

		for (unsigned int i = 0; i < references->size(); ++i)
		{
			ZResource resource = references->operator[](i);

			writer.StartObject();

			writer.String("resourceID");
			writer.String(resource.GetResourceID().c_str());

			writer.String("hash");
			writer.String(ZUtility::ConvertValueToHexString(resource.GetRuntimeResourceID()->GetID()).c_str());

			writer.String("resourceReferenceFlags");
			writer.String(ZUtility::ConvertResourceReferenceFlagsToString(resource.GetResourceReferenceFlags()).c_str());

			writer.EndObject();
		}

		writer.EndArray();

		writer.String("chunks");
		writer.StartArray();

		for (unsigned int i = 0; i < resourceLibraries->size(); ++i)
		{
			const ZResourceLibrary* resourceLibrary = resourceLibraries->operator[](i);
			const IOI::SHeaderLibraryChunk headerLibraryChunk = headerLibrary->chunks[i];

			resourceLibrary->SerializeToJSON(headerLibraryChunk, writer);
		}

		writer.EndArray();

		writer.String("externalResourceIds");
		headerLibrary->externalResourceIds.SerializeToJSON(writer);

		writer.String("ridSource");
		writer.String(ZUtility::ConvertValueToHexString(headerLibrary->ridSource).c_str());

		writer.EndObject();

		jsonOutput = stringBuffer.GetString();
	}

	int ZHeaderLibrary::GetIndexOfResourceLibrary(const IOI::ZRuntimeResourceID& runtimeResourceID, bool isGlobalResourceLibrary) const
	{
		if (isGlobalResourceLibrary)
		{
			for (unsigned int i = 0; i < resourceLibraries->size(); ++i)
			{
				std::vector<ZResource*>* resources = resourceLibraries->operator[](i)->GetResources();

				for (unsigned int j = 0; j < resources->size(); ++j)
				{
					IOI::ZRuntimeResourceID* runtimeResourceID2 = resources->operator[](j)->GetRuntimeResourceID();

					if (runtimeResourceID == *runtimeResourceID2)
					{
						return i;
					}
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < resourceLibraries->size(); ++i)
			{
				IOI::ZRuntimeResourceID* runtimeResourceID2 = resourceLibraries->operator[](i)->GetRuntimeResourceID();

				if (runtimeResourceID.GetLow() == runtimeResourceID2->GetLow())
				{
					return i;
				}
			}
		}

		return -1;
	}

	unsigned int ZHeaderLibrary::GetIndexOfResource(const IOI::ZRuntimeResourceID& runtimeResourceID) const
	{
		for (unsigned int i = 0; i < resourceLibraries->size(); ++i)
		{
			std::vector<ZResource*>* resources = resourceLibraries->operator[](i)->GetResources();

			for (unsigned int j = 0; j < resources->size(); ++j)
			{
				IOI::ZRuntimeResourceID* runtimeResourceID2 = resources->operator[](j)->GetRuntimeResourceID();

				if (runtimeResourceID == *runtimeResourceID2)
				{
					return j;
				}
			}
		}

		return 0;
	}

	void ZHeaderLibrary::ExtractPropertyNames(bool closeFile, bool exit)
	{
		try
		{
			std::set<std::string> propertyNames;
			static std::ofstream textFile("PropertyNames.txt");

			if (exit)
			{
				textFile.close();

				return;
			}

			static IOI::ZBinaryDeserializer binaryDeserializer;

			for (unsigned i = 0; i < resourceLibraries->size(); ++i)
			{
				ZResourceLibrary* resourceLibrary = resourceLibraries->operator[](i);
				std::vector<ZResource*>* resources = resourceLibrary->GetResources();

				for (unsigned int j = 0; j < resources->size(); ++j)
				{
					if (resources->operator[](j)->GetResourceHeaderHeader()->m_type != 0x54424c55)
					{
						continue;
					}

					HM5ResourceTool::IO::ZBinaryReader resourceLibraryBinaryReader = HM5ResourceTool::IO::ZBinaryReader(resources->operator[](j)->GetResourceLibraryPath());

					resourceLibraryBinaryReader.GetInputStream()->Seek(resources->operator[](j)->GetOffsetInResourceLibrary(), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

					unsigned int dataSize = resources->operator[](j)->GetResourceDataSize();
					void* data = resourceLibraryBinaryReader.ReadBytes(dataSize);

					IOI::STemplateEntityBlueprint* templateEntityBlueprint = static_cast<IOI::STemplateEntityBlueprint*>(binaryDeserializer.Deserialize(data, dataSize));

					for (unsigned i = 0; i < templateEntityBlueprint->entityTemplates.Size(); ++i)
					{
						IOI::STemplateSubEntityBlueprint* templateSubEntityBlueprint = &templateEntityBlueprint->entityTemplates[i];

						for (unsigned j = 0; j < templateSubEntityBlueprint->propertyAliases.Size(); ++j)
						{
							IOI::SEntityTemplatePropertyAlias* entityTemplatePropertyAlias = &templateSubEntityBlueprint->propertyAliases[j];

							propertyNames.insert(entityTemplatePropertyAlias->sAliasName.ToCString());
							propertyNames.insert(entityTemplatePropertyAlias->sPropertyName.ToCString());
						}
					}

					operator delete(templateEntityBlueprint, std::align_val_t(binaryDeserializer.GetAlignment()));
				}
			}

			for (auto it = propertyNames.begin(); it != propertyNames.end(); ++it)
			{
				textFile << *it << "\n";
			}

			if (closeFile)
			{
				textFile.close();
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();
		}
	}

	void ZHeaderLibrary::ExtractTypeNames(bool closeFile, bool exit)
	{
		try
		{
			static std::set<std::string> typeNames;
			static std::ofstream textFile("TypeNames_TBLU.txt");

			if (exit)
			{
				textFile.close();

				return;
			}

			static IOI::ZBinaryDeserializer binaryDeserializer;

			for (unsigned i = 0; i < resourceLibraries->size(); ++i)
			{
				ZResourceLibrary* resourceLibrary = resourceLibraries->operator[](i);
				std::vector<ZResource*>* resources = resourceLibrary->GetResources();

				for (unsigned int j = 0; j < resources->size(); ++j)
				{
					/*if (resources->operator[](j)->GetResourceHeaderHeader()->m_type != 0x54454d50)
					{
						continue;
					}

					HM5ResourceTool::IO::ZBinaryReader resourceLibraryBinaryReader = HM5ResourceTool::IO::ZBinaryReader(resources->operator[](j)->GetResourceLibraryPath());

					resourceLibraryBinaryReader.GetInputStream()->Seek(resources->operator[](j)->GetOffsetInResourceLibrary(), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

					unsigned int dataSize = resources->operator[](j)->GetResourceDataSize();
					void* data = resourceLibraryBinaryReader.ReadBytes(dataSize);

					IOI::STemplateEntity* templateEntity = static_cast<IOI::STemplateEntity*>(binaryDeserializer.Deserialize(data, dataSize, nullptr));

					for (unsigned i = 0; i < templateEntity->entityTemplates.Size(); ++i)
					{
						IOI::STemplateSubEntity* templateSubEntity = &templateEntity->entityTemplates[i];

						for (unsigned int j = 0; j < templateSubEntity->propertyValues.Size(); ++j)
						{
							typeNames.insert(templateSubEntity->propertyValues[j].value.GetTypeID()->pTypeInfo->pszTypeName);
						}

						for (unsigned int j = 0; j < templateSubEntity->postInitPropertyValues.Size(); ++j)
						{
							typeNames.insert(templateSubEntity->postInitPropertyValues[j].value.GetTypeID()->pTypeInfo->pszTypeName);
						}
					}

					operator delete(templateEntity, std::align_val_t(binaryDeserializer.GetAlignment()));*/

					if (resources->operator[](j)->GetResourceHeaderHeader()->m_type != 0x54424c55)
					{
						continue;
					}

					HM5ResourceTool::IO::ZBinaryReader resourceLibraryBinaryReader = HM5ResourceTool::IO::ZBinaryReader(resources->operator[](j)->GetResourceLibraryPath());

					resourceLibraryBinaryReader.GetInputStream()->Seek(resources->operator[](j)->GetOffsetInResourceLibrary(), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

					unsigned int dataSize = resources->operator[](j)->GetResourceDataSize();
					void* data = resourceLibraryBinaryReader.ReadBytes(dataSize);

					IOI::STemplateEntityBlueprint* templateEntityBlueprint = static_cast<IOI::STemplateEntityBlueprint*>(binaryDeserializer.Deserialize(data, dataSize));

					for (unsigned i = 0; i < templateEntityBlueprint->entityTemplates.Size(); ++i)
					{
						IOI::STemplateSubEntityBlueprint* templateSubEntityBlueprint = &templateEntityBlueprint->entityTemplates[i];

						for (unsigned j = 0; j < templateSubEntityBlueprint->entitySubsets.Size(); ++j)
						{
							typeNames.insert(templateSubEntityBlueprint->entitySubsets[j].m_value.subsetType->pTypeInfo->pszTypeName);
						}
					}

					operator delete(templateEntityBlueprint, std::align_val_t(binaryDeserializer.GetAlignment()));
				}
			}

			for (auto it = typeNames.begin(); it != typeNames.end(); ++it)
			{
				textFile << *it << "\n";
			}

			if (closeFile)
			{
				textFile.close();
			}
		}
		catch (std::invalid_argument& ex)
		{
			std::cout << ex.what();
		}
		catch (std::ios_base::failure& ex)
		{
			std::cout << ex.what();
		}
	}
}
