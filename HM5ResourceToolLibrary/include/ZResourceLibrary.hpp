#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include "Glacier2/Resource/ZLocalResourceFileMediator.hpp"
#include "Glacier2/Resource/SResourceHeaderHeader.hpp"
#include "Glacier2/Resource/ZResourceHeaderReader.hpp"
#include "Glacier2/HeaderLibrary/SHeaderLibraryChunk.hpp"
#include "IO/ZBinaryReader.hpp"
#include "IO/ZBinaryWriter.hpp"
#include "ZResource.hpp"

namespace HM5ResourceTool
{
	class ZResourceLibrary
	{
	public:
		__declspec(dllexport) ZResourceLibrary() = default;
		__declspec(dllexport) ZResourceLibrary(const std::string& headerLibraryPath, const std::string& gamePath);
		__declspec(dllexport) ~ZResourceLibrary();
		bool ExtractResourceLibrary(IO::ZBinaryReader& headerBinaryReader, IOI::SHeaderLibraryChunk& headerLibraryChunk,
			const std::vector<unsigned int>& resouceOffsetsInResourceLib, const std::string exportFolderPath);
		bool ParseResourceLibrary(IO::ZBinaryReader& headerBinaryReader, IOI::SHeaderLibraryChunk& headerLibraryChunk,
			const std::vector<unsigned int>& resouceOffsetsInResourceLib, bool readData);
		__declspec(dllexport) void LoadResourceData(const unsigned int resourceIndex);
		__declspec(dllexport) std::string GetResourceID() const;
		__declspec(dllexport) std::string GetFilePath() const;
		__declspec(dllexport) std::string GetName() const;
		const IOI::ZRuntimeResourceID* GetRuntimeResourceID() const;
		const IOI::SResourceHeaderHeader* GetResourceHeaderHeader() const;
		const std::vector<ZResource*>* GetResources() const;
		__declspec(dllexport) IOI::ZRuntimeResourceID* GetRuntimeResourceID();
		__declspec(dllexport) IOI::SResourceHeaderHeader* GetResourceHeaderHeader();
		__declspec(dllexport) std::vector<ZResource*>* GetResources();
		void SetResourceID(std::string& resourceID);
		void SetFilePath(std::string& filePath);
		void SetName(std::string& name);
		void SetResourceHeaderHeader(IOI::SResourceHeaderHeader& resourceHeaderHeader);
		void SetRuntimeResourceID(IOI::ZRuntimeResourceID& runtimeResourceID);
		void SetResources(std::vector<ZResource*>* resources);
		void ParseResourceHeader(IO::ZBinaryReader& resourceBinaryReader, ZResource& resource);
		std::string ConvertTypeToString(unsigned int type);
		unsigned int GetResourceFilePosition(IOI::ZRuntimeResourceID& ridResource);
		void SerializeToJSON(const IOI::SHeaderLibraryChunk& headerLibraryChunk, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
		int GetIndexOfResource(const unsigned long long hash);
		void GenerateResourceLibrary();
		std::string GetResourceName(const std::string& resourceID);

	private:
		std::string headerLibraryPath;
		std::string gamePath;
		std::string resourceID;
		std::string filePath;
		std::string name;
		IOI::ZRuntimeResourceID runtimeResourceID;
		IOI::SResourceHeaderHeader resourceHeaderHeader;
		std::vector<ZResource*>* resources;
		std::unordered_map<unsigned long long, unsigned int>* resourceRuntimeResourceIDs;
	};
}
