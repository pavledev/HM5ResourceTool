#include "pch.h"
#include "ZResource.hpp"
#include "ZUtility.hpp"

namespace HM5ResourceTool
{
	ZResource::ZResource()
	{
		resourceID = "";
		offsetInHeaderLibrary = 0;
		offsetInResourceLibrary = 0;
		headerData = nullptr;
		headerDataSize = 0;
		resourceData = nullptr;
		resourceDataSize = 0;
	}

	ZResource::~ZResource()
	{
		if (headerData)
		{
			delete[] headerData;
		}

		if (resourceData)
		{
			delete[] resourceData;
		}
	}

	const std::string ZResource::GetHeaderLibraryPath() const
	{
		return headerLibraryPath;
	}

	const std::string ZResource::GetResourceLibraryPath() const
	{
		return resourceLibraryPath;
	}

	const std::string ZResource::GetName() const
	{
		std::string name;

		if (resourceID.contains("("))
		{
			size_t index = resourceID.substr(0, resourceID.find("(")).find_last_of('/');

			name = resourceID.substr(index, resourceID.find("(") - 1 - index);
		}
		else
		{
			if (resourceID.contains("?"))
			{
				name = resourceID.substr(resourceID.find("?"), resourceID.find("]") - resourceID.find("?"));
			}
			else
			{
				name = resourceID.substr(resourceID.find_last_of("/"), resourceID.find("]") - resourceID.find("/"));
			}
		}

		return name;
	}

	const std::string ZResource::GetName()
	{
		return name;
	}

	IOI::SResourceHeaderHeader* ZResource::GetResourceHeaderHeader()
	{
		return &resourceHeaderHeader;
	}

	const IOI::SResourceHeaderHeader* ZResource::GetResourceHeaderHeader() const
	{
		return &resourceHeaderHeader;
	}

	const IOI::ZRuntimeResourceID* ZResource::GetRuntimeResourceID() const
	{
		return &runtimeResourceID;
	}

	IOI::ZRuntimeResourceID* ZResource::GetRuntimeResourceID()
	{
		return &runtimeResourceID;
	}

	std::string ZResource::GetResourceID() const
	{
		return resourceID;
	}

	const IOI::EResourceReferenceFlags ZResource::GetResourceReferenceFlags() const
	{
		return resourceReferenceFlags;
	}

	IOI::EResourceReferenceFlags ZResource::GetResourceReferenceFlags()
	{
		return resourceReferenceFlags;
	}

	std::string ZResource::GetFileFormat() const
	{
		return ZUtility::ConvertTypeToString(resourceHeaderHeader.m_type);
	}

	const unsigned int ZResource::GetOffsetInHeaderLibrary() const
	{
		return offsetInHeaderLibrary;
	}

	const unsigned int ZResource::GetOffsetInResourceLibrary() const
	{
		return offsetInResourceLibrary;
	}

	const void* ZResource::GetHeaderData() const
	{
		return headerData;
	}

	void* ZResource::GetHeaderData()
	{
		return headerData;
	}

	const unsigned int ZResource::GetHeaderDataSize() const
	{
		return headerDataSize;
	}

	const void* ZResource::GetResourceData() const
	{
		return resourceData;
	}

	void* ZResource::GetResourceData()
	{
		return resourceData;
	}

	const unsigned int ZResource::GetResourceDataSize() const
	{
		return resourceDataSize;
	}

	std::vector<ZResource*>* ZResource::GetReferences()
	{
		return &references;
	}

	const std::vector<ZResource*>* ZResource::GetReferences() const
	{
		return &references;
	}

	void ZResource::SetHeaderLibraryPath(std::string headerLibraryPath)
	{
		this->headerLibraryPath = headerLibraryPath;
	}

	void ZResource::SetResourceLibraryPath(std::string resourceLibraryPath)
	{
		this->resourceLibraryPath = resourceLibraryPath;
	}

	void ZResource::SetName(std::string name)
	{
		this->name = name;
	}

	void ZResource::SetResourceHeaderHeader(const IOI::SResourceHeaderHeader& resourceHeaderHeader)
	{
		this->resourceHeaderHeader = resourceHeaderHeader;
	}

	void ZResource::SetRuntimeResourceID(const IOI::ZRuntimeResourceID& runtimeResourceID)
	{
		this->runtimeResourceID = runtimeResourceID;
	}

	void ZResource::SetResourceID(std::string resourceID)
	{
		this->resourceID = resourceID;
	}

	void ZResource::SetResourceReferenceFlags(const IOI::EResourceReferenceFlags resourceReferenceFlags)
	{
		this->resourceReferenceFlags = resourceReferenceFlags;
	}

	void ZResource::SetOffsetInHeaderLibrary(const unsigned int offsetInHeaderLibrary)
	{
		this->offsetInHeaderLibrary = offsetInHeaderLibrary;
	}

	void ZResource::SetOffsetInResourceLibrary(const unsigned int offsetInResourceLibrary)
	{
		this->offsetInResourceLibrary = offsetInResourceLibrary;
	}

	void ZResource::SetHeaderData(void* headerData)
	{
		this->headerData = headerData;
	}

	void ZResource::SetHeaderDataSize(const unsigned int headerDataSize)
	{
		this->headerDataSize = headerDataSize;
	}

	void ZResource::SetResourceData(void* resourceData)
	{
		this->resourceData = resourceData;
	}

	void ZResource::SetResourceDataSize(const unsigned int resourceDataSize)
	{
		this->resourceDataSize = resourceDataSize;
	}

	void ZResource::AddReference(ZResource* reference)
	{
		references.push_back(reference);
	}

	void ZResource::SetReferences(std::vector<ZResource*> references)
	{
		this->references = references;
	}

	void ZResource::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();

		writer.String("resourceHeaderHeader");
		resourceHeaderHeader.SerializeToJSON(writer);

		writer.String("resourceID");
		writer.String(resourceID.c_str());

		writer.String("hash");
		writer.String(ZUtility::ConvertValueToHexString(runtimeResourceID.GetID()).c_str());

		writer.String("offsetInHeaderLibrary");
		writer.String(ZUtility::ConvertValueToHexString(offsetInHeaderLibrary).c_str());

		writer.String("offsetInResourceLibrary");
		writer.String(ZUtility::ConvertValueToHexString(offsetInResourceLibrary).c_str());

		writer.String("headerDataSize");
		writer.String(ZUtility::ConvertValueToHexString(headerDataSize).c_str());

		writer.String("resourceDataSize");
		writer.String(ZUtility::ConvertValueToHexString(resourceDataSize).c_str());

		writer.String("references");
		writer.StartArray();

		for (unsigned int i = 0; i < references.size(); ++i)
		{
			ZResource* resourceReference = references.operator[](i);
			std::string resourceID = resourceReference->resourceID;

			writer.String("index");
			writer.Uint(i);

			writer.String("resourceHeaderHeader");
			resourceReference->resourceHeaderHeader.SerializeToJSON(writer);

			writer.String("resourceID");
			writer.String(resourceReference->resourceID.c_str());

			writer.String("hash");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->runtimeResourceID.GetID()).c_str());

			writer.String("resourceReferenceFlags");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->resourceReferenceFlags).c_str());

			writer.String("offsetInHeaderLibrary");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->offsetInHeaderLibrary).c_str());

			writer.String("offsetInResourceLibrary");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->offsetInResourceLibrary).c_str());

			writer.String("headerDataSize");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->headerDataSize).c_str());

			writer.String("resourceDataSize");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->resourceDataSize).c_str());
		}

		writer.EndArray();

		writer.EndObject();
	}

	std::string ZResource::SerializeToJSON() const
	{
		rapidjson::StringBuffer stringBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

		writer.StartObject();

		writer.String("headerLibraryPath");
		writer.String(headerLibraryPath.c_str());

		writer.String("resourceLibraryPath");
		writer.String(resourceLibraryPath.c_str());

		writer.String("resourceHeaderHeader");
		resourceHeaderHeader.SerializeToJSON(writer);

		writer.String("resourceID");
		writer.String(resourceID.c_str());

		writer.String("hash");
		writer.String(ZUtility::ConvertValueToHexString(runtimeResourceID.GetID()).c_str());

		writer.String("offsetInHeaderLibrary");
		writer.String(ZUtility::ConvertValueToHexString(offsetInHeaderLibrary).c_str());

		writer.String("offsetInResourceLibrary");
		writer.String(ZUtility::ConvertValueToHexString(offsetInResourceLibrary).c_str());

		writer.String("headerDataSize");
		writer.String(ZUtility::ConvertValueToHexString(headerDataSize).c_str());

		writer.String("resourceDataSize");
		writer.String(ZUtility::ConvertValueToHexString(resourceDataSize).c_str());

		writer.String("references");
		writer.StartArray();

		for (unsigned int i = 0; i < references.size(); ++i)
		{
			ZResource* resourceReference = references.operator[](i);
			std::string resourceID = resourceReference->resourceID;

			writer.StartObject();

			/*writer.String("headerLibraryPath");
			writer.Null();

			writer.String("resourceLibraryPath");
			writer.Null();*/

			writer.String("index");
			writer.Uint(i);

			writer.String("resourceHeaderHeader");
			resourceReference->resourceHeaderHeader.SerializeToJSON(writer);

			writer.String("resourceID");
			writer.String(resourceID.c_str());

			writer.String("hash");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->runtimeResourceID.GetID()).c_str());

			writer.String("resourceReferenceFlags");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->resourceReferenceFlags).c_str());

			writer.String("offsetInHeaderLibrary");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->offsetInHeaderLibrary).c_str());

			writer.String("offsetInResourceLibrary");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->offsetInResourceLibrary).c_str());

			writer.String("headerDataSize");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->headerDataSize).c_str());

			writer.String("resourceDataSize");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->resourceDataSize).c_str());

			writer.EndObject();
		}

		writer.EndArray();

		writer.EndObject();

		return stringBuffer.GetString();
	}

	std::string ZResource::SerializeToJSON()
	{
		rapidjson::StringBuffer stringBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);

		writer.StartObject();

		writer.String("headerLibraryPath");
		writer.String(headerLibraryPath.c_str());

		writer.String("resourceLibraryPath");
		writer.String(resourceLibraryPath.c_str());

		writer.String("resourceHeaderHeader");
		resourceHeaderHeader.SerializeToJSON(writer);

		writer.String("resourceID");
		writer.String(resourceID.c_str());

		writer.String("hash");
		writer.String(ZUtility::ConvertValueToHexString(runtimeResourceID.GetID()).c_str());

		writer.String("offsetInHeaderLibrary");
		writer.String(ZUtility::ConvertValueToHexString(offsetInHeaderLibrary).c_str());

		writer.String("offsetInResourceLibrary");
		writer.String(ZUtility::ConvertValueToHexString(offsetInResourceLibrary).c_str());

		writer.String("headerDataSize");
		writer.String(ZUtility::ConvertValueToHexString(headerDataSize).c_str());

		writer.String("resourceDataSize");
		writer.String(ZUtility::ConvertValueToHexString(resourceDataSize).c_str());

		writer.String("references");
		writer.StartArray();

		for (unsigned int i = 0; i < references.size(); ++i)
		{
			ZResource* resourceReference = references.operator[](i);
			std::string resourceID = resourceReference->resourceID;

			writer.StartObject();

			/*writer.String("headerLibraryPath");
			writer.Null();

			writer.String("resourceLibraryPath");
			writer.Null();*/

			writer.String("index");
			writer.Uint(i);

			writer.String("resourceHeaderHeader");
			resourceReference->resourceHeaderHeader.SerializeToJSON(writer);

			writer.String("resourceID");
			writer.String(resourceID.c_str());

			writer.String("hash");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->runtimeResourceID.GetID()).c_str());

			writer.String("resourceReferenceFlags");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->resourceReferenceFlags).c_str());

			writer.String("offsetInHeaderLibrary");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->offsetInHeaderLibrary).c_str());

			writer.String("offsetInResourceLibrary");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->offsetInResourceLibrary).c_str());

			writer.String("headerDataSize");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->headerDataSize).c_str());

			writer.String("resourceDataSize");
			writer.String(ZUtility::ConvertValueToHexString(resourceReference->resourceDataSize).c_str());

			writer.EndObject();
		}

		writer.EndArray();

		writer.EndObject();

		return stringBuffer.GetString();
	}

	ZResource* ZResource::DeserializeFromJSON(const rapidjson::Document& document)
	{
		ZResource* resource = new ZResource();
		const rapidjson::Value& resourceHeaderHeaderObject = document["resourceHeaderHeader"].GetObj();

		resource->headerLibraryPath = document["headerLibraryPath"].GetString();
		resource->resourceLibraryPath = document["resourceLibraryPath"].GetString();

		std::string type = resourceHeaderHeaderObject["m_type"].GetString();
		std::string referencesChunkSize = resourceHeaderHeaderObject["m_nReferencesChunkSize"].GetString();
		std::string statesChunkSize = resourceHeaderHeaderObject["m_nStatesChunkSize"].GetString();
		std::string dataSize = resourceHeaderHeaderObject["m_nDataSize"].GetString();
		std::string systemMemoryRequirement = resourceHeaderHeaderObject["m_nSystemMemoryRequirement"].GetString();
		std::string videoMemoryRequirement = resourceHeaderHeaderObject["m_nVideoMemoryRequirement"].GetString();

		resource->resourceHeaderHeader.m_type = ZUtility::ConvertStringToType(type);
		resource->resourceHeaderHeader.m_nReferencesChunkSize = std::strtoul(referencesChunkSize.substr(2).c_str(), 0, 16);
		resource->resourceHeaderHeader.m_nStatesChunkSize = std::strtoul(statesChunkSize.substr(2).c_str(), 0, 16);
		resource->resourceHeaderHeader.m_nDataSize = std::strtoul(dataSize.substr(2).c_str(), 0, 16);
		resource->resourceHeaderHeader.m_nSystemMemoryRequirement = std::strtoul(systemMemoryRequirement.substr(2).c_str(), 0, 16);
		resource->resourceHeaderHeader.m_nVideoMemoryRequirement = std::strtoul(videoMemoryRequirement.substr(2).c_str(), 0, 16);

		resource->runtimeResourceID = IOI::ZRuntimeResourceID::Create(strtoull(document["hash"].GetString(), 0, 16));
		resource->resourceID = document["resourceID"].GetString();
		resource->offsetInHeaderLibrary = std::strtoul(document["offsetInHeaderLibrary"].GetString(), 0, 10);
		resource->offsetInResourceLibrary = std::strtoul(document["offsetInResourceLibrary"].GetString(), 0, 10);
		resource->headerDataSize = std::strtoul(document["headerDataSize"].GetString(), 0, 10);
		resource->resourceDataSize = std::strtoul(document["resourceDataSize"].GetString(), 0, 10);

		const rapidjson::Value& array = document["references"].GetArray();

		//resource->references = std::vector<ZResource*>(array.Size());

		for (Value::ConstValueIterator it = array.Begin(); it != array.End(); ++it)
		{
			const rapidjson::Value& resourceObject = it->GetObj();
			const rapidjson::Value& resourceHeaderHeaderObject2 = resourceObject["resourceHeaderHeader"].GetObj();
			ZResource* reference = new ZResource();

			//reference->headerLibraryPath = document["headerLibraryPath"].GetString();
			//reference->resourceLibraryPath = document["resourceLibraryPath"].GetString();

			type = resourceHeaderHeaderObject2["m_type"].GetString();
			referencesChunkSize = resourceHeaderHeaderObject2["m_nReferencesChunkSize"].GetString();
			statesChunkSize = resourceHeaderHeaderObject2["m_nStatesChunkSize"].GetString();
			dataSize = resourceHeaderHeaderObject2["m_nDataSize"].GetString();
			systemMemoryRequirement = resourceHeaderHeaderObject2["m_nSystemMemoryRequirement"].GetString();
			videoMemoryRequirement = resourceHeaderHeaderObject2["m_nVideoMemoryRequirement"].GetString();

			reference->resourceHeaderHeader.m_type = ZUtility::ConvertStringToType(type);
			reference->resourceHeaderHeader.m_nReferencesChunkSize = std::strtoul(referencesChunkSize.substr(2).c_str(), 0, 16);
			reference->resourceHeaderHeader.m_nStatesChunkSize = std::strtoul(statesChunkSize.substr(2).c_str(), 0, 16);
			reference->resourceHeaderHeader.m_nDataSize = std::strtoul(dataSize.substr(2).c_str(), 0, 10);
			reference->resourceHeaderHeader.m_nSystemMemoryRequirement = std::strtoul(systemMemoryRequirement.substr(2).c_str(), 0, 16);
			reference->resourceHeaderHeader.m_nVideoMemoryRequirement = std::strtoul(videoMemoryRequirement.substr(2).c_str(), 0, 16);

			reference->runtimeResourceID = IOI::ZRuntimeResourceID::Create(strtoull(resourceObject["hash"].GetString(), 0, 16));
			reference->resourceID = resourceObject["resourceID"].GetString();
			reference->resourceReferenceFlags = static_cast<IOI::EResourceReferenceFlags>(strtoull(resourceObject["resourceReferenceFlags"].GetString(), 0, 16));
			reference->offsetInHeaderLibrary = std::strtoul(resourceObject["offsetInHeaderLibrary"].GetString(), 0, 16);
			reference->offsetInResourceLibrary = std::strtoul(resourceObject["offsetInResourceLibrary"].GetString(), 0, 16);
			reference->headerDataSize = std::strtoul(resourceObject["headerDataSize"].GetString(), 0, 16);
			reference->resourceDataSize = std::strtoul(resourceObject["resourceDataSize"].GetString(), 0, 16);

			resource->references.push_back(reference);
		}

		return resource;
	}

	const std::string ZResource::GetResourceDetails()
	{
		std::stringstream stringStream;

		stringStream << "- File format: " << ZUtility::ConvertTypeToString(resourceHeaderHeader.m_type) << std::endl;
		stringStream << "- References chunk size: " << ZUtility::ConvertValueToHexString(resourceHeaderHeader.m_nReferencesChunkSize) << std::endl;
		stringStream << "- States chunk size: " << ZUtility::ConvertValueToHexString(resourceHeaderHeader.m_nStatesChunkSize) << std::endl;
		stringStream << "- Data size: " << ZUtility::ConvertValueToHexString(resourceHeaderHeader.m_nDataSize) << std::endl;
		stringStream << "- System memory requirement: " << ZUtility::ConvertValueToHexString(resourceHeaderHeader.m_nSystemMemoryRequirement) << std::endl;
		stringStream << "- Video memory requirement: " << ZUtility::ConvertValueToHexString(resourceHeaderHeader.m_nVideoMemoryRequirement) << std::endl << std::endl;

		stringStream << "- Resource ID: " << resourceID << std::endl;
		stringStream << "- Hash: " << ZUtility::ConvertValueToHexString(runtimeResourceID.GetID()) << std::endl;
		stringStream << "- Offset in header library: " << ZUtility::ConvertValueToHexString(offsetInHeaderLibrary) << std::endl;
		stringStream << "- Offset in resource library: " << ZUtility::ConvertValueToHexString(offsetInResourceLibrary) << std::endl;
		stringStream << "- Header data size: " << ZUtility::ConvertValueToHexString(headerDataSize) << std::endl;
		stringStream << "- Resource data size: " << ZUtility::ConvertValueToHexString(resourceDataSize) << std::endl << std::endl;

		stringStream << "Depends on " << references.size() << " resources:" << std::endl;

		for (unsigned int i = 0; i < references.size(); ++i)
		{
			ZResource* resourceReference = references.operator[](i);
			std::string resourceID = resourceReference->resourceID;

			stringStream << " - Index: " << i << std::endl;
			stringStream << " - File format: " << ZUtility::ConvertTypeToString(resourceReference->resourceHeaderHeader.m_type) << std::endl;
			stringStream << " - References chunk size: " << ZUtility::ConvertValueToHexString(resourceReference->resourceHeaderHeader.m_nReferencesChunkSize) << std::endl;
			stringStream << " - States chunk size: " << ZUtility::ConvertValueToHexString(resourceReference->resourceHeaderHeader.m_nStatesChunkSize) << std::endl;
			stringStream << " - Data size: " << ZUtility::ConvertValueToHexString(resourceReference->resourceHeaderHeader.m_nDataSize) << std::endl;
			stringStream << " - System memory requirement: " << ZUtility::ConvertValueToHexString(resourceReference->resourceHeaderHeader.m_nSystemMemoryRequirement) << std::endl;
			stringStream << " - Video memory requirement: " << ZUtility::ConvertValueToHexString(resourceReference->resourceHeaderHeader.m_nVideoMemoryRequirement) << std::endl << std::endl;

			stringStream << " - Resource ID: " << resourceID << std::endl;
			stringStream << " - Hash: " << ZUtility::ConvertValueToHexString(resourceReference->runtimeResourceID.GetID()) << std::endl;
			stringStream << " - Offset in header library: " << ZUtility::ConvertValueToHexString(resourceReference->offsetInHeaderLibrary) << std::endl;
			stringStream << " - Offset in resource library: " << ZUtility::ConvertValueToHexString(resourceReference->offsetInResourceLibrary) << std::endl;
			stringStream << " - Header data size: " << ZUtility::ConvertValueToHexString(resourceReference->headerDataSize) << std::endl;
			stringStream << " - Resource data size: " << ZUtility::ConvertValueToHexString(resourceReference->resourceDataSize) << std::endl << std::endl;

			if (i < references.size() - 1)
			{
				stringStream << " ----------------------------------------------------------" << std::endl << std::endl;
			}
		}

		return stringStream.str();
	}

	const bool ZResource::IsResourceDataLoaded()
	{
		return resourceData;
	}

	const bool ZResource::IsResourceDataLoaded() const
	{
		return resourceData;
	}

	const std::string ZResource::GetHexBytesFromHeaderLibrary()
	{
		return GetHexBytes(static_cast<unsigned char*>(headerData), headerDataSize);
	}

	const std::string ZResource::GetHexBytesFromResourceLibrary()
	{
		return GetHexBytes(static_cast<unsigned char*>(resourceData), resourceDataSize);
	}

	const std::string ZResource::GetHexBytes(const unsigned char* data, unsigned int dataSize)
	{
		std::stringstream result;

		result << "Offset   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F Decoded Text" << std::endl;

		for (unsigned int i = 0; i < dataSize; i += 16)
		{
			result << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << i << " ";

			for (unsigned int j = 0; j < 16; ++j)
			{
				if (i + j < dataSize)
				{
					result << std::setw(2) << static_cast<unsigned int>(data[i + j]) << " ";
				}
				else
				{
					result << "   ";
				}
			}

			result << std::setw(0) << std::setfill(' ') << std::dec << std::nouppercase;

			for (unsigned int j = 0; j < 16; ++j)
			{
				if (i + j < dataSize)
				{
					if (!std::isprint(data[i + j]))
					{
						result << ".";
					}
					else
					{
						result << static_cast<char>(data[i + j]);
					}
				}
			}

			result << std::endl;
		}

		return result.str();
	}
}
